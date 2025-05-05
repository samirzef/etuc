# Guide d'installation et d'exécution

## 1. Configuration du débogueur (VS Code)

Dans le fichier `app/launch.json`, configurez le chemin vers `gdb.exe` comme suit :

```json
{
  "miDebuggerPath": "C:/winlibs/mingw64/bin/gdb.exe"
}
```

Assurez-vous que ce chemin correspond bien à l'emplacement où vous avez installé **MinGW**.

---

## 2. Compilation

Pour compiler le projet, exécutez la commande suivante dans le terminal :

```bash
g++ -std=c++17 graph.cpp tsp.cpp kerdjidj_mohamedSeddik_zeghouf_samir.cpp -o kerdjidj_mohamedSeddik_zeghouf_samir
```

Cela générera un exécutable nommé `kerdjidj_mohamedSeddik_zeghouf_samir`.

---

## 3. Exécution

Une fois la compilation réussie, lancez le programme avec :

```bash
./kerdjidj_mohamedSeddik_zeghouf_samir
```

---

## Remarques

* Assurez-vous que `g++` est installé et ajouté à la variable d'environnement PATH de votre système.
* Ce projet nécessite au minimum la norme **C++17**.
* Si vous êtes sous Windows et rencontrez des problèmes d'exécution, vous pouvez également lancer :

```bash
kerdjidj_mohamedSeddik_zeghouf_samir.exe
```
