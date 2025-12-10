#pragma once
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <tinyxml2.h>
#include <string>
#include <fstream>
#include <iostream>

using json = nlohmann::json;
using namespace tinyxml2;

namespace raym{

	struct Layer
	{
	    std::vector<int> tiles;
	    int id;
	    std::string name;
	    std::string type;
	    bool visible;
	    int width;
	    int height;
	    int x;
	    int y;
	    std::string tileSource;
	   	Texture2D texture;
	   	int tilesetColumns;
	};

	struct InfoTilset{
		std::string imageSource;
		int tilesetColumns;
	};

	InfoTilset getImageSource(std::string tsxSource){
		XMLDocument doc;
		doc.LoadFile(tsxSource.c_str());

		auto *tileset = doc.FirstChildElement("tileset");
		auto tilesetColumns = tileset->IntAttribute("columns");
		auto *image = tileset->FirstChildElement("image");

		auto imageSource = image->Attribute("source");
		std::cout<<"Image source contenue dans xml: "<<imageSource<<std::endl;
		std::cout<<"Columns: "<<tilesetColumns<<std::endl;
		return {imageSource,tilesetColumns};
	}

	class Map{
	private:
		std::vector<Layer> layers;
		std::string mapPath;
		int tileWidth;
		int tileHeight;

	public:
		Map(std::string mapPath){
			this->mapPath = mapPath;
		}

		~Map(){
			for(auto &layer: layers){
				UnloadTexture(layer.texture);
			}
		}

		void loadMap(){
			std::ifstream file(mapPath);
			json j;
			file >> j;

			tileWidth = j["tilewidth"];
			tileHeight = j["tileheight"];

			for(auto &layer : j["layers"]){
				Layer current_layer;
				std::string imageSource = j["tilesets"][layer["id"].get<int>() - 1]["source"];
				auto infotilset = getImageSource(imageSource);

				
				current_layer.tiles = layer["data"].get<std::vector<int>>();
				current_layer.id = layer["id"].get<int>();
				current_layer.name = layer["name"].get<std::string>();
				current_layer.type = layer["type"].get<std::string>();
				current_layer.visible = layer["visible"].get<bool>();
				current_layer.width = layer["width"].get<int>();
				current_layer.height = layer["height"].get<int>();
				current_layer.x = layer["x"].get<int>();
				current_layer.y = layer["y"].get<int>();
				current_layer.tileSource = infotilset.imageSource;
				current_layer.texture = LoadTexture(current_layer.tileSource.c_str());
				current_layer.tilesetColumns = infotilset.tilesetColumns;

				layers.push_back(current_layer);
			}
		}

		void drawMap(){
    		for (auto &layer : layers) {

		        if (!layer.visible) continue;

		        for (int y = 0; y < layer.height; y++) {
		            for (int x = 0; x < layer.width; x++) {

		                int tileIndex = layer.tiles[y * layer.width + x];

		                if (tileIndex == 0) continue; // 0 = vide dans Tiled

		                // Tiled utilise GID → il faut retirer firstgid mais ici on assume 1
		                tileIndex -= 1;

		                int tx = (tileIndex % layer.tilesetColumns) * tileWidth; // tilesetColumns
		                int ty = (tileIndex / layer.tilesetColumns) * tileHeight;

		                Rectangle source = { (float)tx, (float)ty, (float)tileWidth, (float)tileHeight };
		                Rectangle dest = { 
		                    (float)(x * tileWidth), 
		                    (float)(y * tileHeight), 
		                    (float)tileWidth, 
		                    (float)tileHeight 
		                };

                		DrawTextureRec(layer.texture, source, {dest.x, dest.y}, WHITE);
		            }
		        }

		    }
		}

	};

}
