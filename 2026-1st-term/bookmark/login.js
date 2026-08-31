import { auth } from './firebase-init.js';
import {
  onAuthStateChanged,
  signInWithEmailAndPassword,
  createUserWithEmailAndPassword,
} from 'https://www.gstatic.com/firebasejs/10.12.2/firebase-auth.js';

// ── Se já estiver logado, vai direto para o app ───────────
onAuthStateChanged(auth, user => {
  if (user) window.location.href = 'index.html';
});

// ── Modo: 'login' | 'register' ────────────────────────────
let mode = 'login';

document.getElementById('tab-login').addEventListener('click',    () => setMode('login'));
document.getElementById('tab-register').addEventListener('click', () => setMode('register'));

function setMode(m) {
  mode = m;
  document.getElementById('tab-login').classList.toggle('active',    m === 'login');
  document.getElementById('tab-register').classList.toggle('active', m === 'register');
  document.getElementById('auth-label').textContent = m === 'login' ? 'Entrar' : 'Criar conta';
  document.getElementById('auth-password').setAttribute(
    'autocomplete', m === 'login' ? 'current-password' : 'new-password'
  );
  hideError();
}

// ── Mostrar/ocultar senha ─────────────────────────────────
document.getElementById('btn-toggle-pass').addEventListener('click', () => {
  const inp = document.getElementById('auth-password');
  inp.type  = inp.type === 'password' ? 'text' : 'password';
});

// ── Envio do formulário ───────────────────────────────────
document.getElementById('form-auth').addEventListener('submit', async e => {
  e.preventDefault();

  const email    = document.getElementById('auth-email').value.trim();
  const password = document.getElementById('auth-password').value;

  if (!email || !password) return;

  setLoading(true);
  hideError();

  try {
    if (mode === 'login') {
      await signInWithEmailAndPassword(auth, email, password);
    } else {
      await createUserWithEmailAndPassword(auth, email, password);
    }
    // onAuthStateChanged redireciona para index.html
  } catch (err) {
    showError(friendlyMsg(err.code));
    setLoading(false);
  }
});

// ── Helpers de UI ─────────────────────────────────────────
function setLoading(on) {
  document.getElementById('btn-auth').disabled = on;
  document.getElementById('auth-label').classList.toggle('hidden', on);
  document.getElementById('auth-spinner').classList.toggle('hidden', !on);
}

function showError(msg) {
  const el = document.getElementById('auth-error');
  el.textContent = msg;
  el.classList.remove('hidden');
}

function hideError() {
  document.getElementById('auth-error').classList.add('hidden');
}

// ── Tradução dos códigos de erro do Firebase ──────────────
function friendlyMsg(code) {
  const map = {
    'auth/user-not-found':        'E-mail não cadastrado.',
    'auth/wrong-password':        'Senha incorreta.',
    'auth/invalid-email':         'E-mail inválido.',
    'auth/invalid-credential':    'E-mail ou senha incorretos.',
    'auth/email-already-in-use':  'E-mail já cadastrado. Faça login.',
    'auth/weak-password':         'Senha muito fraca (mínimo 6 caracteres).',
    'auth/too-many-requests':     'Muitas tentativas. Aguarde alguns minutos.',
    'auth/network-request-failed':'Erro de conexão. Verifique sua internet.',
    'auth/operation-not-allowed': 'Login por e-mail não está ativado no Firebase.',
  };
  return map[code] ?? `Erro inesperado (${code}). Tente novamente.`;
}
