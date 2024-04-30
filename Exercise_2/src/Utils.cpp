#include "Utils.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary{

bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv", mesh))
    {
        return false;
    }
    return true;
}

bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coordinates;
        char sep;

        converter >> id >> sep >> marker >> sep >> coordinates(0) >> sep >> coordinates(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coordinates);

        if(marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }
    file.close();
    return true;
}

bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;
        char sep;

        converter >> id >> sep >> marker >> sep >> vertices(0) >> sep >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }
    file.close();
    return true;
}

bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        unsigned int id, marker, numVertices, numEdges;
        char sep;

        converter >> id >> sep >> marker >> sep >> numVertices >> sep;

        vector<unsigned int> vertices(numVertices);

        for(unsigned int i = 0; i < numVertices; i++){
            converter >> vertices[i] >> sep;
        }

        converter >> sep >> numEdges >> sep;

        vector<unsigned int> edges(numEdges);

        for(unsigned int i = 0; i < numEdges; i++){
            converter >> edges[i] >> sep;
        }

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
    }
    file.close();
    return true;
}

//Test
bool EdgesControl(const PolygonalMesh& mesh){
    for (unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {
        const vector<unsigned int>& vertices = mesh.Cell2DVertices[c];
        unsigned int numVertices = vertices.size();

        // Controllo che ci siano almeno 3 vertici per formare un poligono
        if (numVertices < 3) {
            cerr << "The polygon " << c << " has less than 3 vertices." << endl;
            return false;
        }

        for (unsigned int i = 0; i < numVertices; ++i)
        {
            unsigned int vertex1 = vertices[i];
            unsigned int vertex2 = vertices[(i + 1) % numVertices]; // Faccio questo in modo che l'ultimo vertice risulti collegato al primo

            // Calcolo la distanza tra i due vertici
            Vector2d coordinates1 = mesh.Cell0DCoordinates[vertex1];
            Vector2d coordinates2 = mesh.Cell0DCoordinates[vertex2];
            double distance = sqrt(pow(coordinates2.x() - coordinates1.x(), 2) + pow(coordinates2.y() - coordinates1.y(), 2));

            // Verifico se la distanza è nulla o molto vicina a zero (considerando una tolleranza)
            if (distance < 1e-16)
            {
                cerr << "Distance between the vertices of the polygon " << c << " too short or null." << endl;
                return false;
            }
        }
    }
    return true;
}

bool AreasControl(const PolygonalMesh& mesh)
{
    for (unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {
        const vector<unsigned int>& vertices = mesh.Cell2DVertices[c];
        unsigned int numVertices = vertices.size();

        // Controllo che ci siano almeno 3 vertici per formare un poligono
        if (numVertices < 3) {
            cerr << "The polygon " << c << " has less than 3 vertices." << endl;
            return false;
        }

        double sum = 0;
        double area_polygon = 0;
        for (unsigned int i = 0; i < numVertices; ++i)
        {
            unsigned int vertex1 = vertices[i];
            unsigned int vertex2 = vertices[(i + 1) % numVertices];

            Vector2d coordinates1 = mesh.Cell0DCoordinates[vertex1];
            Vector2d coordinates2 = mesh.Cell0DCoordinates[vertex2];

            double sumi = (coordinates1.x()*coordinates2.y())-(coordinates2.x()*coordinates1.y());
            sum = sum + sumi;
        }
        area_polygon = 0.5 * abs(sum);
        if (area_polygon < 1e-16)
        {
            cerr << "Area of the polygon " << c << " too short or null." << endl;
            return false;
        }
    }
    return true;
}
}

