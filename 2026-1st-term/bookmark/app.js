// ── Firebase ──────────────────────────────────────────────
import { auth, db } from "./firebase-init.js";
import {
  onAuthStateChanged,
  signOut,
} from "https://www.gstatic.com/firebasejs/10.12.2/firebase-auth.js";
import {
  collection,
  doc,
  onSnapshot,
  addDoc,
  updateDoc,
  deleteDoc,
  setDoc,
  getDoc,
  writeBatch,
} from "https://www.gstatic.com/firebasejs/10.12.2/firebase-firestore.js";

// ════════════════════════════════════════════════════════════
//  CONSTANTS
// ════════════════════════════════════════════════════════════

const CAT_COLORS = [
  "#388bfd",
  "#3fb950",
  "#f85149",
  "#d29922",
  "#bc8cff",
  "#79c0ff",
  "#56d364",
  "#ffa657",
  "#e3b341",
  "#db6d28",
];

const DEFAULT_CAT = "Geral";
const INIT_CATS = [
  DEFAULT_CAT,
  "Trabalho",
  "Estudo",
  "Entretenimento",
  "Tecnologia",
  "Pessoal",
];

// ════════════════════════════════════════════════════════════
//  CONFIG CHECK — avisa se firebase-config.js não foi preenchido
// ════════════════════════════════════════════════════════════

import { FIREBASE_CONFIG } from "./firebase-config.js";

if (FIREBASE_CONFIG.apiKey === "SUA_API_KEY") {
  setLoadingMsg(
    "⚙ Configure o firebase-config.js com suas credenciais do Firebase.",
  );
  throw new Error("Firebase config placeholder — edit firebase-config.js");
}

// ════════════════════════════════════════════════════════════
//  STATE
// ════════════════════════════════════════════════════════════

const S = {
  bookmarks: [],
  categories: [...INIT_CATS],
  filter: "all",
  search: "",
  sort: "newest",
  view: "grid",
  editId: null,
};

let userId = null;
let unsubBM = null; // unsubscribe bookmarks listener
let unsubCt = null; // unsubscribe categories listener

// ════════════════════════════════════════════════════════════
//  AUTH GUARD
// ════════════════════════════════════════════════════════════

onAuthStateChanged(auth, (user) => {
  if (!user) {
    window.location.href = "login.html";
    return;
  }
  userId = user.uid;
  document.getElementById("user-email").textContent = user.email;
  subscribeToData();
});

// ════════════════════════════════════════════════════════════
//  FIRESTORE — real-time listeners
// ════════════════════════════════════════════════════════════

function subscribeToData() {
  // Categorias
  unsubCt = onSnapshot(
    doc(db, "users", userId, "settings", "data"),
    async (snap) => {
      if (snap.exists()) {
        S.categories = snap.data().categories ?? [...INIT_CATS];
      } else {
        // Primeiro acesso: inicializa categorias padrão
        await setDoc(doc(db, "users", userId, "settings", "data"), {
          categories: [...INIT_CATS],
        });
        S.categories = [...INIT_CATS];
      }
      renderSidebar();
    },
    (err) => toast("Erro ao carregar categorias: " + err.message, "err"),
  );

  // Bookmarks (sync em tempo real entre dispositivos)
  unsubBM = onSnapshot(
    collection(db, "users", userId, "bookmarks"),
    (snap) => {
      S.bookmarks = snap.docs.map((d) => ({ id: d.id, ...d.data() }));
      hideLoading();
      renderSidebar();
      renderBookmarks();
    },
    (err) => toast("Erro ao carregar bookmarks: " + err.message, "err"),
  );
}

// ════════════════════════════════════════════════════════════
//  CRUD  (async — Firestore)
// ════════════════════════════════════════════════════════════

async function createBookmark(data) {
  await addDoc(collection(db, "users", userId, "bookmarks"), {
    title: data.title.trim(),
    url: data.url.trim(),
    description: (data.description ?? "").trim(),
    category: data.category,
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString(),
  });
}

async function updateBookmark(id, data) {
  await updateDoc(doc(db, "users", userId, "bookmarks", id), {
    title: data.title.trim(),
    url: data.url.trim(),
    description: (data.description ?? "").trim(),
    category: data.category,
    updatedAt: new Date().toISOString(),
  });
}

async function deleteBookmark(id) {
  await deleteDoc(doc(db, "users", userId, "bookmarks", id));
}

async function saveCategories(cats) {
  await setDoc(doc(db, "users", userId, "settings", "data"), {
    categories: cats,
  });
}

async function createCategory(name) {
  name = name.trim();
  if (!name) return { ok: false, msg: "Nome não pode ser vazio." };
  if (S.categories.includes(name))
    return { ok: false, msg: `"${name}" já existe.` };
  if (name.length > 30) return { ok: false, msg: "Máximo de 30 caracteres." };
  await saveCategories([...S.categories, name]);
  return { ok: true };
}

async function deleteCategory(name) {
  if (name === DEFAULT_CAT) return;
  const batch = writeBatch(db);
  const toReassign = S.bookmarks.filter((b) => b.category === name);
  toReassign.forEach((b) => {
    batch.update(doc(db, "users", userId, "bookmarks", b.id), {
      category: DEFAULT_CAT,
    });
  });
  const newCats = S.categories.filter((c) => c !== name);
  batch.set(doc(db, "users", userId, "settings", "data"), {
    categories: newCats,
  });
  await batch.commit();
  if (S.filter === name) S.filter = "all";
}

// ════════════════════════════════════════════════════════════
//  HELPERS
// ════════════════════════════════════════════════════════════

function favicon(url) {
  try {
    return `https://www.google.com/s2/favicons?domain=${new URL(url).hostname}&sz=32`;
  } catch {
    return "";
  }
}

function domain(url) {
  try {
    return new URL(url).hostname.replace(/^www\./, "");
  } catch {
    return url;
  }
}

function catColor(name) {
  let h = 0;
  for (const c of name) h = (h * 31 + c.charCodeAt(0)) >>> 0;
  return CAT_COLORS[h % CAT_COLORS.length];
}

function catCount(name) {
  return S.bookmarks.filter((b) => b.category === name).length;
}

function getFiltered() {
  let list = S.bookmarks.filter((b) => {
    if (S.filter !== "all" && b.category !== S.filter) return false;
    if (S.search) {
      const q = S.search.toLowerCase();
      return (
        b.title.toLowerCase().includes(q) ||
        b.url.toLowerCase().includes(q) ||
        b.description.toLowerCase().includes(q)
      );
    }
    return true;
  });

  switch (S.sort) {
    case "oldest":
      list.sort((a, b) => a.createdAt.localeCompare(b.createdAt));
      break;
    case "alpha":
      list.sort((a, b) => a.title.localeCompare(b.title, "pt"));
      break;
    default:
      list.sort((a, b) => b.createdAt.localeCompare(a.createdAt));
      break;
  }
  return list;
}

function esc(s) {
  return String(s)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;");
}

function validateURL(url) {
  if (!url) return "URL é obrigatória.";
  try {
    new URL(url);
    return "";
  } catch {
    return "URL inválida — inclua https://";
  }
}

// ════════════════════════════════════════════════════════════
//  RENDER
// ════════════════════════════════════════════════════════════

function renderSidebar() {
  const ul = document.getElementById("cat-nav");
  ul.innerHTML = "";

  const row = (value, label, count, color) => {
    const li = document.createElement("li");
    li.className = "cat-item" + (S.filter === value ? " active" : "");
    li.dataset.cat = value;
    li.innerHTML = `
      <span class="cat-dot" style="background:${color}"></span>
      <span>${label}</span>
      <span class="cat-count">${count}</span>`;
    li.addEventListener("click", () => {
      S.filter = value;
      renderSidebar();
      renderBookmarks();
    });
    ul.appendChild(li);
  };

  row("all", "Todos", S.bookmarks.length, "#7d8590");
  S.categories.forEach((c) => row(c, c, catCount(c), catColor(c)));
}

function renderBookmarks() {
  const box = document.getElementById("bm-container");
  const empty = document.getElementById("empty-state");
  const cntEl = document.getElementById("results-count");

  box.className = `view-${S.view}`;

  const list = getFiltered();
  cntEl.textContent = list.length
    ? `${list.length} bookmark${list.length > 1 ? "s" : ""}${S.search ? ` para "${S.search}"` : ""}`
    : "";

  if (!list.length) {
    box.innerHTML = "";
    empty.classList.remove("hidden");
    document.getElementById("empty-sub").innerHTML = S.search
      ? `Nenhum resultado para <strong>"${S.search}"</strong>.`
      : `Clique em <strong>+ Novo Bookmark</strong> para começar.`;
    return;
  }

  empty.classList.add("hidden");
  box.innerHTML =
    S.view === "grid"
      ? list.map(cardHTML).join("")
      : list.map(rowHTML).join("");
}

function cardHTML(bm) {
  const fav = favicon(bm.url);
  const dom = domain(bm.url);
  const col = catColor(bm.category);
  const ltr = bm.title.charAt(0).toUpperCase();
  return `
<div class="bm-card" data-id="${bm.id}">
  <div class="card-top">
    <img  class="card-fav" src="${fav}" alt=""
          onerror="this.classList.add('hidden');this.nextElementSibling.classList.remove('hidden')">
    <div  class="card-fav-fb hidden">${ltr}</div>
    <div  class="card-text">
      <a class="card-title" href="${bm.url}" target="_blank" rel="noopener" title="${esc(bm.title)}">${esc(bm.title)}</a>
      <div class="card-domain">${dom}</div>
    </div>
  </div>
  ${bm.description ? `<p class="card-desc">${esc(bm.description)}</p>` : ""}
  <div class="card-foot">
    <span class="cat-tag">
      <span class="cat-dot" style="background:${col}"></span>
      <span>${esc(bm.category)}</span>
    </span>
    <div class="card-actions">
      <button class="btn-icon edit" data-id="${bm.id}" title="Editar">✏</button>
      <button class="btn-icon del"  data-id="${bm.id}" title="Excluir">🗑</button>
    </div>
  </div>
</div>`;
}

function rowHTML(bm) {
  const fav = favicon(bm.url);
  const dom = domain(bm.url);
  const col = catColor(bm.category);
  const ltr = bm.title.charAt(0).toUpperCase();
  return `
<div class="bm-row" data-id="${bm.id}">
  <img  class="row-fav" src="${fav}" alt=""
        onerror="this.classList.add('hidden');this.nextElementSibling.classList.remove('hidden')">
  <div  class="card-fav-fb hidden" style="width:16px;height:16px;font-size:9px;flex-shrink:0">${ltr}</div>
  <a    class="row-title" href="${bm.url}" target="_blank" rel="noopener">${esc(bm.title)}</a>
  <span class="row-domain">${dom}</span>
  <span class="cat-tag">
    <span class="cat-dot" style="background:${col}"></span>
    <span>${esc(bm.category)}</span>
  </span>
  <div class="card-actions">
    <button class="btn-icon edit" data-id="${bm.id}" title="Editar">✏</button>
    <button class="btn-icon del"  data-id="${bm.id}" title="Excluir">🗑</button>
  </div>
</div>`;
}

function renderCatModal() {
  const ul = document.getElementById("cat-manage-list");
  ul.innerHTML = "";
  S.categories.forEach((cat) => {
    const cnt = catCount(cat);
    const isD = cat === DEFAULT_CAT;
    const li = document.createElement("li");
    li.className = "cmi" + (isD ? " default" : "");
    li.innerHTML = `
      <span class="cmi-label">
        <span class="cat-dot" style="background:${catColor(cat)}"></span>
        <span>${esc(cat)}</span>
      </span>
      <span class="cmi-count">${cnt} BM${cnt !== 1 ? "s" : ""}</span>
      <button class="btn-del-cat" data-cat="${esc(cat)}"
        ${isD ? 'disabled title="Categoria padrão"' : `title="Excluir ${esc(cat)}"`}>🗑</button>`;
    ul.appendChild(li);
  });
}

function populateCatSelect(selected) {
  document.getElementById("f-cat").innerHTML = S.categories
    .map(
      (c) =>
        `<option value="${c}"${c === selected ? " selected" : ""}>${c}</option>`,
    )
    .join("");
}

// ════════════════════════════════════════════════════════════
//  MODALS
// ════════════════════════════════════════════════════════════

const openModal = (id) =>
  document.getElementById(id).classList.remove("hidden");
const closeModal = (id) => document.getElementById(id).classList.add("hidden");

function openBookmarkModal(bm = null) {
  S.editId = bm?.id ?? null;
  document.getElementById("modal-bm-title").textContent = bm
    ? "Editar Bookmark"
    : "Novo Bookmark";
  document.getElementById("f-url").value = bm?.url ?? "";
  document.getElementById("f-title").value = bm?.title ?? "";
  document.getElementById("f-desc").value = bm?.description ?? "";
  ["f-url", "f-title"].forEach((id) =>
    document.getElementById(id).classList.remove("err"),
  );
  ["err-url", "err-title"].forEach((id) => {
    document.getElementById(id).textContent = "";
  });
  populateCatSelect(
    bm?.category ?? (S.filter !== "all" ? S.filter : DEFAULT_CAT),
  );
  openModal("modal-bm");
  setTimeout(() => document.getElementById("f-url").focus(), 60);
}

// Padrão callback para evitar múltiplos listeners de confirmação
let _confirmCb = null;

function openConfirm(msg, cb) {
  _confirmCb = cb;
  document.getElementById("confirm-msg").textContent = msg;
  openModal("modal-confirm");
}

// ════════════════════════════════════════════════════════════
//  TOAST
// ════════════════════════════════════════════════════════════

function toast(msg, type = "ok") {
  const el = document.createElement("div");
  el.className = `toast t-${type}`;
  el.textContent = msg;
  document.body.appendChild(el);
  requestAnimationFrame(() =>
    requestAnimationFrame(() => el.classList.add("show")),
  );
  setTimeout(() => {
    el.classList.remove("show");
    setTimeout(() => el.remove(), 300);
  }, 2800);
}

// ════════════════════════════════════════════════════════════
//  LOADING SCREEN
// ════════════════════════════════════════════════════════════

function hideLoading() {
  document.getElementById("loading-overlay").classList.add("hidden");
}

function setLoadingMsg(msg) {
  const el = document.getElementById("loading-msg");
  if (el) el.textContent = msg;
}

// ════════════════════════════════════════════════════════════
//  EVENTS
// ════════════════════════════════════════════════════════════

function initEvents() {
  // ── Novo bookmark ─────────────────────────────────────────
  document
    .getElementById("btn-new")
    .addEventListener("click", () => openBookmarkModal());

  // ── Logout ────────────────────────────────────────────────
  document.getElementById("btn-logout").addEventListener("click", async () => {
    if (unsubBM) unsubBM();
    if (unsubCt) unsubCt();
    await signOut(auth);
    // onAuthStateChanged redireciona para login.html
  });

  // ── Busca ─────────────────────────────────────────────────
  const searchEl = document.getElementById("search-input");
  const clearBtn = document.getElementById("btn-clear");

  searchEl.addEventListener("input", () => {
    S.search = searchEl.value;
    clearBtn.classList.toggle("hidden", !S.search);
    renderBookmarks();
  });
  clearBtn.addEventListener("click", () => {
    S.search = searchEl.value = "";
    clearBtn.classList.add("hidden");
    searchEl.focus();
    renderBookmarks();
  });

  // ── Ordenação ─────────────────────────────────────────────
  document
    .getElementById("sort-select")
    .addEventListener("change", function () {
      S.sort = this.value;
      renderBookmarks();
    });

  // ── Visualização ──────────────────────────────────────────
  document.getElementById("btn-grid").addEventListener("click", () => {
    S.view = "grid";
    document.getElementById("btn-grid").classList.add("active");
    document.getElementById("btn-list").classList.remove("active");
    renderBookmarks();
  });
  document.getElementById("btn-list").addEventListener("click", () => {
    S.view = "list";
    document.getElementById("btn-list").classList.add("active");
    document.getElementById("btn-grid").classList.remove("active");
    renderBookmarks();
  });

  // ── Gerenciar categorias ──────────────────────────────────
  document.getElementById("btn-manage-cats").addEventListener("click", () => {
    renderCatModal();
    openModal("modal-cats");
    setTimeout(() => document.getElementById("new-cat-input").focus(), 60);
  });

  // ── Fechar modais (data-close) ────────────────────────────
  document
    .querySelectorAll("[data-close]")
    .forEach((btn) =>
      btn.addEventListener("click", () => closeModal(btn.dataset.close)),
    );
  document.querySelectorAll(".modal-bg").forEach((bg) =>
    bg.addEventListener("click", (e) => {
      if (e.target === bg) closeModal(bg.id);
    }),
  );

  // ── Formulário de bookmark ────────────────────────────────
  document.getElementById("form-bm").addEventListener("submit", async (e) => {
    e.preventDefault();
    const url = document.getElementById("f-url").value.trim();
    const title = document.getElementById("f-title").value.trim();
    const desc = document.getElementById("f-desc").value.trim();
    const cat = document.getElementById("f-cat").value;

    let ok = true;
    const uErr = validateURL(url);
    if (uErr) {
      document.getElementById("err-url").textContent = uErr;
      document.getElementById("f-url").classList.add("err");
      ok = false;
    } else {
      document.getElementById("err-url").textContent = "";
      document.getElementById("f-url").classList.remove("err");
    }
    if (!title) {
      document.getElementById("err-title").textContent =
        "Título é obrigatório.";
      document.getElementById("f-title").classList.add("err");
      ok = false;
    } else {
      document.getElementById("err-title").textContent = "";
      document.getElementById("f-title").classList.remove("err");
    }
    if (!ok) return;

    const btn = e.target.querySelector('[type="submit"]');
    btn.disabled = true;

    try {
      const data = { url, title, description: desc, category: cat };
      if (S.editId) {
        await updateBookmark(S.editId, data);
        toast("Bookmark atualizado!", "info");
      } else {
        await createBookmark(data);
        toast("Bookmark adicionado! 🔖");
      }
      closeModal("modal-bm");
    } catch (err) {
      toast("Erro ao salvar: " + err.message, "err");
    } finally {
      btn.disabled = false;
    }
  });

  // ── Auto-preenche título com o domínio da URL ─────────────
  document.getElementById("f-url").addEventListener("blur", function () {
    const titleEl = document.getElementById("f-title");
    if (titleEl.value) return;
    const d = domain(this.value);
    if (d && d !== this.value)
      titleEl.value = d.charAt(0).toUpperCase() + d.slice(1);
  });

  // ── Ações nos cards (delegação) ───────────────────────────
  document
    .getElementById("bm-container")
    .addEventListener("click", async (e) => {
      const btn = e.target.closest(".btn-icon");
      if (!btn) return;
      const id = btn.dataset.id;
      if (btn.classList.contains("edit")) {
        const bm = S.bookmarks.find((b) => b.id === id);
        if (bm) openBookmarkModal(bm);
      } else if (btn.classList.contains("del")) {
        const bm = S.bookmarks.find((b) => b.id === id);
        if (!bm) return;
        openConfirm(
          `Excluir "${bm.title}"? Esta ação não pode ser desfeita.`,
          async () => {
            try {
              await deleteBookmark(id);
              toast("Bookmark excluído.", "err");
            } catch (err) {
              toast("Erro ao excluir: " + err.message, "err");
            }
          },
        );
      }
    });

  // ── Confirmar exclusão ────────────────────────────────────
  document.getElementById("btn-confirm").addEventListener("click", async () => {
    if (_confirmCb) {
      await _confirmCb();
      _confirmCb = null;
    }
    closeModal("modal-confirm");
  });
  document.getElementById("btn-cancel").addEventListener("click", () => {
    _confirmCb = null;
    closeModal("modal-confirm");
  });

  // ── Adicionar categoria ───────────────────────────────────
  const newCatInput = document.getElementById("new-cat-input");
  const doAddCat = async () => {
    const name = newCatInput.value.trim();
    const res = await createCategory(name);
    if (!res.ok) {
      toast(res.msg, "err");
      return;
    }
    newCatInput.value = "";
    toast(`Categoria "${name}" criada!`);
  };
  document.getElementById("btn-add-cat").addEventListener("click", doAddCat);
  newCatInput.addEventListener("keydown", (e) => {
    if (e.key === "Enter") doAddCat();
  });

  // ── Excluir categoria (delegação) ─────────────────────────
  document.getElementById("cat-manage-list").addEventListener("click", (e) => {
    const btn = e.target.closest(".btn-del-cat");
    if (!btn || btn.disabled) return;
    const cat = btn.dataset.cat;
    const cnt = catCount(cat);
    openConfirm(
      cnt > 0
        ? `Excluir "${cat}"? Os ${cnt} bookmark(s) serão movidos para "${DEFAULT_CAT}".`
        : `Excluir a categoria "${cat}"?`,
      async () => {
        try {
          await deleteCategory(cat);
          renderCatModal();
          closeModal("modal-cats");
          toast(`Categoria "${cat}" excluída.`, "err");
        } catch (err) {
          toast("Erro ao excluir categoria: " + err.message, "err");
        }
      },
    );
  });

  // ── Atalhos de teclado ────────────────────────────────────
  document.addEventListener("keydown", (e) => {
    if ((e.ctrlKey || e.metaKey) && e.key === "k") {
      e.preventDefault();
      searchEl.focus();
      searchEl.select();
    }
    if ((e.ctrlKey || e.metaKey) && e.key === "n") {
      const anyOpen = [...document.querySelectorAll(".modal-bg")].some(
        (m) => !m.classList.contains("hidden"),
      );
      if (!anyOpen) {
        e.preventDefault();
        openBookmarkModal();
      }
    }
    if (e.key === "Escape") {
      for (const id of ["modal-confirm", "modal-cats", "modal-bm"]) {
        if (!document.getElementById(id).classList.contains("hidden")) {
          closeModal(id);
          break;
        }
      }
    }
  });
}

// ════════════════════════════════════════════════════════════
//  INIT
// ════════════════════════════════════════════════════════════

initEvents();
