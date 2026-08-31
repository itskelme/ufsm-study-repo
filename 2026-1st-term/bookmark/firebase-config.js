// ═══════════════════════════════════════════════════════════
//  CONFIGURAÇÃO DO FIREBASE
//  ─────────────────────────────────────────────────────────
//  1. Acesse https://console.firebase.google.com
//  2. Crie um projeto (ou use um existente)
//  3. Vá em Configurações do Projeto › Web › Adicionar app (ícone </>)
//  4. Copie o objeto firebaseConfig e cole abaixo
//  5. No Console, ative:
//       Authentication › Sign-in method › E-mail/senha  ✓
//       Firestore Database › Criar banco (modo produção) ✓
//  6. Em Firestore › Regras, cole as regras do README.md
// ═══════════════════════════════════════════════════════════

// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyBBjVoUKcajtni9gV-b50GLf9vRM4QK5Xg",
  authDomain: "trabalho-rose.firebaseapp.com",
  projectId: "trabalho-rose",
  storageBucket: "trabalho-rose.firebasestorage.app",
  messagingSenderId: "949718428814",
  appId: "1:949718428814:web:f4dfd516a2430e573baddf",
  measurementId: "G-DRVT9D2CK4",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
