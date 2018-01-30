#include "MyFace.h"

MyFace::MyFace(OSG::Pnt3f centre, std::vector<OSG::Pnt3f> vertices) {
	faceCentre = centre;
	faceVertices = vertices;
};

MyFace::~MyFace() {

};

OSG::Pnt3f MyFace::getFaceCentre() {
	return faceCentre;
};

void MyFace::setFaceCentre(OSG::Pnt3f centre) {
	faceCentre = centre;
};

std::vector<OSG::Pnt3f> MyFace::getFaceVertices() {
	return faceVertices;
};

void MyFace::setFaceVertex(int index, OSG::Pnt3f point) {
	faceVertices[index] = point;
};