#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary{

struct PolygonalMesh
{
    // Celle 0D: Vertici; Celle 1D: Lati; Celle 2D: Poligoni
    unsigned int NumberCell0D = 0; ///< Numero di celle 0D
    std::vector<unsigned int> Cell0DId = {}; ///< Id delle celle 0D, dimensione 1 x Numero di celle 0D
    std::vector<Vector2d> Cell0DCoordinates = {}; ///< Vettore di vettori di due double contenenti le coordinate delle celle 0D, taglia 2 x Numero di celle 0D (x,y)
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; ///< Associazione degli Id delle celle 0D al corrispondente marker

    unsigned int NumberCell1D = 0; ///< Numero di celle 1D
    std::vector<unsigned int> Cell1DId = {}; ///< Id delle celle 1D, dimensione 1 x Numero di celle 1D
    std::vector<Vector2i> Cell1DVertices = {}; ///< Vettore di vettori di due interi contenenti gli Id dei vertici di ciascuna cella 1D, dimensione 2 x Numero di celle 1D (daId,aId)
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; ///< Associazione degli Id delle celle 1D al corrispondente marker

    unsigned int NumberCell2D = 0; ///< Numero di celle 2D
    std::vector<unsigned int> Cell2DId = {}; ///< Id delle celle 2D, dimensione 1 x Numero di celle 2D
    std::vector<vector<unsigned int>> Cell2DVertices = {}; ///< Vettore di vettori contenenti gli Id dei vertici di ciascuna cella 2D
    std::vector<vector<unsigned int>> Cell2DEdges = {}; ///< Vettore di vettori contenenti gli Id dei lati di ciascuna cella 2D
};

}

