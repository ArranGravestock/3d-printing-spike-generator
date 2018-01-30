//MyFace.h

#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>

class MyFace {

	OSG::Pnt3f faceCentre;
	std::vector<OSG::Pnt3f> faceVertices;

public:
	MyFace(OSG::Pnt3f centre, std::vector<OSG::Pnt3f> vertices);
	~MyFace();
	OSG::Pnt3f getFaceCentre();
	void setFaceCentre(OSG::Pnt3f centre);
	
	std::vector<OSG::Pnt3f> getFaceVertices();
	void setFaceVertex(int index, OSG::Pnt3f point);
};