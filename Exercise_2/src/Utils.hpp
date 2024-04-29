#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary{

/// Importa la mesh poligonale e testa se la mesh è corretta
/// Ritorna il risultato della lettura, vero se ha avuto successo, falso altrimenti
bool ImportMesh(const string &filepath,
                PolygonalMesh& mesh);

/// Importa le proprietà delle celle 0D dal file Cell0Ds.csv
/// Ritorna il risultato della lettura, vero se ha avuto successo, falso altrimenti
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Importa le proprietà delle celle 1D dal file Cell1Ds.csv
/// Ritorna il risultato della lettura, vero se ha avuto successo, falso altrimenti
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Importa le proprietà delle celle 2D dal file Cell2Ds.csv
/// Ritorna il risultato della lettura, vero se ha avuto successo, falso altrimenti
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh);

bool EdgesControl(const PolygonalMesh& mesh);

bool AreasControl(const PolygonalMesh& mesh);

}
