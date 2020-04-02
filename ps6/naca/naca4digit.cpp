#include <polygonalmesh.h>
#include "naca4digit.h"
#include <math.h>

std::vector <YsVec3> MakeNACA4DigitAirfoil(int naca,const double dz)
{
	std::vector <YsVec3> vtx;
	double f = (naca/100) % 10;
	double p = f/10;
	double m = (double)(naca/1000) / 100;
	double t = (double)(naca%100) / 100;
	double c = 1;

	for (double z=0; z<1; z+=dz)
	{
		double yt = (5*t) * (0.2969*pow(z, 0.5) - 0.1260*z - 0.3516*pow(z, 2) + 0.2843*pow(z, 3) - 0.1015*pow(z, 4));
		double edge_pos = z/c;

		double yc;
		if (0<=z && z<p*c)
		{
			yc = (m / pow(p, 2)) * (2 * p * edge_pos - pow(edge_pos, 2));
		}
		if (p*c<=z && z<=c)
		{
			yc = (m / pow((1 - p), 2)) * ((1 - 2 * p) + 2 * p * edge_pos - pow(edge_pos, 2));
		}

		const double x = 0;
		const double y = yc + yt;
		vtx.push_back(YsVec3(x, y, z));
	}

	for (double z=1; z>0; z-=dz)
	{
		double yt = (5*t) * (0.2969*pow(z, 0.5) - 0.1260*z-0.3516*pow(z, 2) + 0.2843*pow(z, 3) - 0.1015*pow(z, 4));
		double edge_pos = z/c;

		double yc;
		if (0<=z && z<p*c)
		{
			yc = (m / pow(p, 2)) * (2*p*edge_pos - pow(edge_pos, 2));
		}
		if (p*c<=z && z<=c)
		{
			yc = (m / pow((1-p), 2)) * ((1-2*p) + 2*p* edge_pos - pow(edge_pos, 2));
		}

		const double x = 0;
		const double y = yc - yt;
		vtx.push_back(YsVec3(x, y, z));
	}

	return vtx;
}

void MakeNACA4DigitAirfoilWing(class PolygonalMesh &mesh,int naca,float span,float sweepBack,float dihedral,float taper,float dz)
{
	std::vector <YsVec3> vertex_zeroCS;
	vertex_zeroCS = MakeNACA4DigitAirfoil(naca,dz);

	std::vector <YsVec3> vertex_posCS;
	for (int i=0; i<vertex_posCS.size(); i++)
	{
		float val1 = vertex_posCS[i][0]*taper + span;
		float val2 = vertex_posCS[i][1]*taper + dihedral;
		float val3 = vertex_posCS[i][2]*taper + sweepBack;
		vertex_posCS.push_back(YsVec3(val1, val2, val3));
	}

	std::vector <YsVec3> vertex_negCS;
	for (int i=0; i<vertex_negCS.size(); i++)
	{
		float val1 = vertex_negCS[i][0]*taper - span;
		float val2 = vertex_negCS[i][1]*taper + dihedral;
		float val3 = vertex_negCS[i][2]*taper + sweepBack;
		vertex_negCS.push_back(YsVec3(val1, val2, val3));
	}

	for (int i=0; i<vertex_zeroCS.size(); i+=1)
	{
        // Postive Span Quadrilateral - Triangle 1
		std::vector <PolygonalMesh::VertexHandle> positiveTriangle1Hd;
		positiveTriangle1Hd.push_back(mesh.AddVertex(vertex_zeroCS[i]));
		positiveTriangle1Hd.push_back(mesh.AddVertex(vertex_zeroCS[i+1]));
		positiveTriangle1Hd.push_back(mesh.AddVertex(vertex_posCS[i]));

		auto polygonHd = mesh.AddPolygon(positiveTriangle1Hd);
		auto v1 = vertex_zeroCS[i+1] - vertex_zeroCS[i];
		auto v2 = vertex_posCS[i] - vertex_zeroCS[i+1];
		auto norm = YsUnitVector(v1^v2);
		mesh.SetPolygonNormal(polygonHd,norm);
		// mesh.SetPolygonColor(polyHd, YsGreen());
		
		// Positive Span Quadrliateral - Triangle 2
		std::vector <PolygonalMesh::VertexHandle> positiveTriangle2Hd;
		positiveTriangle2Hd.push_back(mesh.AddVertex(vertex_posCS[i]));
		positiveTriangle2Hd.push_back(mesh.AddVertex(vertex_posCS[i+1]));
		positiveTriangle2Hd.push_back(mesh.AddVertex(vertex_zeroCS[i+1]));

		polygonHd = mesh.AddPolygon(positiveTriangle2Hd);
		v1 = vertex_posCS[i] - vertex_posCS[i+1];
		v2 = vertex_zeroCS[i+1] - vertex_posCS[i];
		norm = YsUnitVector(v1^v2);
		mesh.SetPolygonNormal(polygonHd,norm);


		

        // Negative Span Quadrilateral - Triangle 1
		std::vector <PolygonalMesh::VertexHandle> negativeTriangle1Hd;
		negativeTriangle1Hd.push_back(mesh.AddVertex(vertex_zeroCS[i+1]));
		negativeTriangle1Hd.push_back(mesh.AddVertex(vertex_zeroCS[i]));
		negativeTriangle1Hd.push_back(mesh.AddVertex(vertex_negCS[i]));

		polygonHd = mesh.AddPolygon(negativeTriangle1Hd);
		v1 = vertex_zeroCS[i+1] - vertex_zeroCS[i];
		v2 = vertex_zeroCS[i+1] - vertex_negCS[i];
		norm = YsUnitVector(v1^v2);
		mesh.SetPolygonNormal(polygonHd,norm);


        
		// Negative Span Quadrilateral - Triangle 2
		std::vector <PolygonalMesh::VertexHandle> negativeTriangle2Hd;
		negativeTriangle2Hd.push_back(mesh.AddVertex(vertex_negCS[i]));
		negativeTriangle2Hd.push_back(mesh.AddVertex(vertex_negCS[i+1]));
		negativeTriangle2Hd.push_back(mesh.AddVertex(vertex_zeroCS[i+1]));

		polygonHd = mesh.AddPolygon(negativeTriangle2Hd);
		v1 = vertex_negCS[i+1] - vertex_zeroCS[i+1];
		v2 = vertex_zeroCS[i+1] - vertex_negCS[i];
		norm = YsUnitVector(v1^v2);
		mesh.SetPolygonNormal(polygonHd,norm);

	}


}
