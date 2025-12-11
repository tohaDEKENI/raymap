# raymap

## Description
RayMap est une bibliothèque C++ pour charger et afficher des **cartes 2D depuis Tiled** (JSON + TSX) avec Raylib.  
Elle gère les **tilesets multiples**, les **layers**, et fournit les rectangles de tiles pour les collisions ou la physique.

---

## Fonctionnalités
- Chargement des fichiers JSON exportés par Tiled.  
- Support des **tilesets multiples** et récupération des textures via TSX.  
- Dessin des maps avec Raylib (`DrawTexturePro`).  
- Gestion de la visibilité des layers.  
- Récupération des rectangles des tiles pour collisions (`getTiledRecs()`).

---

## Structure

### `raym::TilesetInfo`
- `firstgid`, `source`, `imageSource`, `columns`, `texture`

### `raym::Layer`
- `tiles`, `id`, `name`, `type`, `visible`, `width`, `height`, `x`, `y`

### `raym::Map`
- `Map(const std::string& mapPath)` : constructeur
- `void loadMap()` : charge tilesets et layers
- `void drawMap()` : dessine la map
- `std::vector<std::vector<Rectangle>> getTiledRecs()` : retourne les rectangles des tiles

---

## Exemple d'utilisation
```cpp
#include "Map.hpp"
#include <raylib.h>

int main() {
    InitWindow(800, 600, "RayMap Test");
    SetTargetFPS(60);

    raym::Map map("map.json");
    map.loadMap();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        map.drawMap(); // Dessine la map

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

## Dépendances

RayMap nécessite les bibliothèques suivantes :

- **[Raylib](https://www.raylib.com/)** — rendu graphique 2D/3D  
- **[nlohmann/json](https://github.com/nlohmann/json)** — parsing des fichiers JSON exportés par Tiled  
- **[TinyXML2](https://github.com/leethomason/tinyxml2)** — lecture des fichiers `.tsx` des tilesets  
- Un compilateur C++ compatible C++17 (g++, clang++, MSVC)

Assurez-vous que Raylib, TinyXML2 et nlohmann/json sont correctement installés sur votre système.

---

## Compilation

Commande standard pour compiler un programme utilisant RayMap :

```bash
g++ main.cpp -o game -I include -ltinyxml2 $(pkg-config --cflags --libs raylib)
