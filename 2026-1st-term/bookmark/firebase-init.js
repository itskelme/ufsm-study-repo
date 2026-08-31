import { initializeApp }  from 'https://www.gstatic.com/firebasejs/10.12.2/firebase-app.js';
import { getAuth }        from 'https://www.gstatic.com/firebasejs/10.12.2/firebase-auth.js';
import { getFirestore }   from 'https://www.gstatic.com/firebasejs/10.12.2/firebase-firestore.js';
import { FIREBASE_CONFIG } from './firebase-config.js';

const firebaseApp = initializeApp(FIREBASE_CONFIG);

export const auth = getAuth(firebaseApp);
export const db   = getFirestore(firebaseApp);
