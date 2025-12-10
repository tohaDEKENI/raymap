#pragma once
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <tinyxml2.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using json = nlohmann::json;
using namespace tinyxml2;

namespace raym {
    
    struct TilesetInfo {
        int firstgid;
        std::string source;
        std::string imageSource;
        int columns;
        Texture2D texture;
    };
    
    struct Layer {
        std::vector<int> tiles;
        int id;
        std::string name;
        std::string type;
        bool visible;
        int width;
        int height;
        int x;
        int y;
    };
    
    class Map {
    private:
        std::vector<Layer> layers;
        std::vector<TilesetInfo> tilesets;
        std::string mapPath;
        int tileWidth;
        int tileHeight;

        std::vector<std::vector<Rectangle>> tiledRecs;
        
        // Charge les informations d'un fichier TSX
        void loadTsxInfo(const std::string& tsxPath, TilesetInfo& tileset) {
            XMLDocument doc;
            if (doc.LoadFile(tsxPath.c_str()) != XML_SUCCESS) {
                std::cerr << "Erreur lors du chargement du fichier TSX: " << tsxPath << std::endl;
                return;
            }
            
            auto* tilesetElement = doc.FirstChildElement("tileset");
            if (!tilesetElement) {
                std::cerr << "Pas d'élément tileset trouvé dans: " << tsxPath << std::endl;
                return;
            }
            
            tileset.columns = tilesetElement->IntAttribute("columns");
            
            auto* image = tilesetElement->FirstChildElement("image");
            if (image) {
                tileset.imageSource = image->Attribute("source");
                std::cout << "Tileset chargé - Image: " << tileset.imageSource 
                          << ", Columns: " << tileset.columns << std::endl;
            }
        }
        
        // Trouve le tileset correspondant à un GID donné
        const TilesetInfo* getTilesetForGid(int gid) const {
            if (gid == 0) return nullptr;
            
            // Trouve le tileset avec le firstgid le plus grand qui est <= gid
            const TilesetInfo* result = nullptr;
            for (const auto& ts : tilesets) {
                if (ts.firstgid <= gid) {
                    if (!result || ts.firstgid > result->firstgid) {
                        result = &ts;
                    }
                }
            }
            return result;
        }
        
    public:
        Map(const std::string& mapPath) : mapPath(mapPath), tileWidth(0), tileHeight(0) {}
        
        ~Map() {
            for (auto& tileset : tilesets) {
                UnloadTexture(tileset.texture);
            }
        }
        
        void loadMap() {

            std::ifstream file(mapPath);
            if (!file.is_open()) {
                std::cerr << "Impossible d'ouvrir le fichier: " << mapPath << std::endl;
                return;
            }
            
            json j;
            file >> j;
            
            tileWidth = j["tilewidth"];
            tileHeight = j["tileheight"];
            
            // Charge tous les tilesets
            for (const auto& tilesetJson : j["tilesets"]) {
                TilesetInfo tileset;
                tileset.firstgid = tilesetJson["firstgid"];
                tileset.source = tilesetJson["source"];
                
                // Charge les infos du fichier TSX
                loadTsxInfo(tileset.source, tileset);
                
                // Charge la texture
                if (!tileset.imageSource.empty()) {
                    tileset.texture = LoadTexture(tileset.imageSource.c_str());
                    if (tileset.texture.id == 0) {
                        std::cerr << "Erreur lors du chargement de la texture: " 
                                  << tileset.imageSource << std::endl;
                    }
                }
                
                tilesets.push_back(tileset);
            }
            
            // Charge tous les layers
            for (const auto& layerJson : j["layers"]) {
                if (layerJson["type"] != "tilelayer") continue;
                
                Layer layer;
                layer.tiles = layerJson["data"].get<std::vector<int>>();
                layer.id = layerJson["id"].get<int>();
                layer.name = layerJson["name"].get<std::string>();
                layer.type = layerJson["type"].get<std::string>();
                layer.visible = layerJson["visible"].get<bool>();
                layer.width = layerJson["width"].get<int>();
                layer.height = layerJson["height"].get<int>();
                layer.x = layerJson["x"].get<int>();
                layer.y = layerJson["y"].get<int>();
                
                layers.push_back(layer);
            }
            
            std::cout << "Carte chargée: " << layers.size() << " layers, " 
                      << tilesets.size() << " tilesets" << std::endl;
            tiledRecs.resize(layers.size());
        }
        
        void drawMap() {
            for (size_t layerIndex = 0; layerIndex < layers.size(); layerIndex++) {
                const auto& layer = layers[layerIndex];
                if (!layer.visible) continue;
                
                for (int y = 0; y < layer.height; y++) {
                    for (int x = 0; x < layer.width; x++) {
                        int gid = layer.tiles[y * layer.width + x];
                        if (gid == 0) continue; // Tile vide
                        
                        // Trouve le tileset correspondant à ce GID
                        const TilesetInfo* tileset = getTilesetForGid(gid);
                        if (!tileset || tileset->texture.id == 0) continue;
                        
                        // Calcule l'index local dans le tileset
                        int localId = gid - tileset->firstgid;
                        
                        // Calcule la position dans le tileset
                        int tx = (localId % tileset->columns) * tileWidth;
                        int ty = (localId / tileset->columns) * tileHeight;
                        
                        Rectangle source = {
                            (float)tx, 
                            (float)ty, 
                            (float)tileWidth, 
                            (float)tileHeight
                        };
                        
                        Rectangle dest = {
                            (float)(layer.x + x * tileWidth),
                            (float)(layer.y + y * tileHeight),
                            (float)tileWidth,
                            (float)tileHeight
                        };
                        
                        tiledRecs[layerIndex].push_back(dest);
                        
                        DrawTexturePro(tileset->texture, source, dest, {0, 0}, 0.0f, WHITE);
                    }
                }
            }
        }
        
        std::vector<std::vector<Rectangle>> getTiledRecs(){
            return tiledRecs;
        }
    };
}