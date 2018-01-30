// Printing
//
// This tutorial will 
// allow you to implement an algorithm
// for 3D printing


// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGTriangleIterator.h>
#include <OpenSG/OSGFaceIterator.h>
#include <OpenSG/OSGLineIterator.h>
#include <math.h>
#include "MyVertex.h"


// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;
OSG::NodeRefPtr file;

int setupGLUT( int *argc, char *argv[] );
OSG::NodeRefPtr drawSpikes(float distance, float depth, OSG::NodeRefPtr node);
void calculateVertexList(OSG::NodeRefPtr node);

OSG::NodeRefPtr scene;
std::vector<MyVertex> vertices;

// GLUT Menu items
enum MENU_ITEMS
{
        WIREFRAME,
        SOLID,
		VERTEX,
	    FLAT,
		SMOOTH,
};

int main(int argc, char **argv)
{
	
    // initialise OpenSG
    OSG::osgInit(argc,argv);

    // initialise GLUT
    int winid = setupGLUT(&argc, argv);

    {
        // create a OSGGLUT window
        OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
        gwin->setGlutId(winid);
        gwin->init();
    
	    scene = OSG::Node::create();
		scene->setCore(OSG::Group::create());		

		//read the file which will be passed as an argument
	
		if(argc > 1)
		{
			for (int i=1; i<argc ; i++){
				std::string option = argv[i];
				if (option == "-f")
					file = OSG::SceneFileHandler::the()->read(argv[i+1]);
			}
		}else{
			file = OSG::makeBox(1,1,1,1,1,1);
		}

		file = OSG::makeTeapot(2.0f, 1.0f);
		auto spikes = drawSpikes(0.1f, 0.3f, file);

		scene->addChild(file);
		scene->addChild(spikes);
		
		//commit all changes to OpenSG 
        OSG::commitChanges();
    
        // create the SimpleSceneManager helper
        mgr = OSG::SimpleSceneManager::create();
    
        // tell the manager what to manage
        mgr->setWindow(gwin );
        mgr->setRoot  (scene);
    
        // show the whole scene
        mgr->showAll();
		//mgr->setHighlight(geometry);

    }
    
    // GLUT main loop
    glutMainLoop();


    return 0;
}

//
// GLUT callback functions
//

// redraw the window
void display(void)
{
	mgr->redraw();
}

// react to size changes
void reshape(int w, int h)
{
    mgr->resize(w, h);
    glutPostRedisplay();
}

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{
    if (!state)
      mgr->mouseButtonPress(button, x, y);
	        
    glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{
    mgr->mouseMove(x, y);
    glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 'mm':
        {
            // clean up global variables
            mgr = NULL;
        
            OSG::osgExit();
            exit(0);
        }
        break;
        case 's':
        {
            mgr->setStatistics(!mgr->getStatistics());
        }
        break;

    }
}

// GLUT menu handling function definition
void menu(int item)
{
        switch (item)
        {
        case VERTEX:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_POINT );
					}
					break;
		case WIREFRAME:{
						   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					   }
					   break;
        case SOLID:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
					}
					   break;

        case FLAT:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
						   glShadeModel( GL_FLAT );
					}
					   break;
        case SMOOTH:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
						   glShadeModel( GL_SMOOTH );
					}
					   break;
        default:
                {            }
                break;
        }

        glutPostRedisplay();

        return;
}
void createMenu(){
	glutCreateMenu(menu);
	// Add menu items
    glutAddMenuEntry("Show vertices", VERTEX);
    glutAddMenuEntry("Show wireframe", WIREFRAME);
    glutAddMenuEntry("Show solid", SOLID);
	glutAddMenuEntry("Shading flat", FLAT);
	glutAddMenuEntry("Shading smooth", SMOOTH);

	// Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return;
}
// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("08Printing Tutorial");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

	//we will create a menu to show different views of the geometry
	createMenu();

    return winid;
}

void calculateVertexList(OSG::NodeRefPtr node) {
	OSG::GeometryRefPtr geocore = dynamic_cast<OSG::Geometry *>(node->getCore());

	//Assign all the position values to our vector of vertices.
	OSG::GeoVectorProperty *pos = geocore->getPositions();
	for (OSG::UInt32 i = 0; i < pos->size(); i++) {
		OSG::Pnt3f p;
		pos->getValue(p, i);

		MyVertex vertex = MyVertex(p);
		vertices.push_back(vertex);
	}

	OSG::GeometryRefPtr geoCore = dynamic_cast<OSG::Geometry*>(node->getCore());

	for (OSG::FaceIterator faceIterator = geoCore->beginFaces(); faceIterator != geoCore->endFaces(); ++faceIterator) {
		for (int i = 0; i < faceIterator.getLength(); i++) {
			for (int j = 0; j < vertices.size(); j++) {
				if (faceIterator.getPosition(i) == vertices[j].getVertex()) {

					std::vector<OSG::Pnt3f> faceIteratorVertices;
					float x = 0, y = 0, z = 0;
					for (int i = 0; i < faceIterator.getLength(); i++) {
						faceIteratorVertices.push_back(faceIterator.getPosition(i));

						x += faceIterator.getPosition(i).x();
						y += faceIterator.getPosition(i).y();
						z += faceIterator.getPosition(i).z();
					}

					OSG::Pnt3f centre(x / faceIterator.getLength(), y / faceIterator.getLength(), z / faceIterator.getLength());

					MyFace face = MyFace(centre, faceIteratorVertices);

					vertices[j].addFace(face);
					//std::cout << "face added: " << face.getFaceCentre() << " at " << j << " where i = " << i << std::endl;
				}
			}
		}
	}
}

OSG::NodeRefPtr drawSpikes(float distance, float depth, OSG::NodeRefPtr node) {

	OSG::NodeRefPtr geonode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_TRIANGLES);

	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	OSG::GeoPnt3fPropertyRefPtr pnts = OSG::GeoPnt3fProperty::create();
	OSG::GeoVec3fPropertyRefPtr norms = OSG::GeoVec3fProperty::create();
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();

	calculateVertexList(node);

	int faceCount = 0;
	int indicesPtr = 0;

	for (std::vector<MyVertex>::iterator myVertexIterator = vertices.begin(); 
		myVertexIterator != vertices.end(); 
		myVertexIterator++) {
		
		//std::cout << vertices.size() << std::endl;

		auto faces = myVertexIterator->getFaces();

		for (std::vector<MyFace>::iterator myFaceIterator = faces.begin();
			myFaceIterator != faces.end();
			myFaceIterator++) {

			faceCount++;

			auto faceVertices = myFaceIterator->getFaceVertices();
			auto faceCentre = myFaceIterator->getFaceCentre();

			std::vector<OSG::Pnt3f> points;
			for each (auto vertex in faceVertices) {
				OSG::Vec3f disA = faceVertices[1] - faceVertices[0];
				OSG::Vec3f disB = faceVertices[2] - faceVertices[0];

				OSG::Vec3f disNorm = disA.cross(disB);
				auto point = vertex + disNorm * distance;
				//std::cout << "vertex: " << vertex << " "<< "point: " << point << std::endl;
				points.push_back(point);
				pnts->addValue(point);
			}

			OSG::Vec3f centreA = points[1] - points[0];
			OSG::Vec3f centreB = points[2] - points[0];

			OSG::Vec3f centreNorm = centreA.cross(centreB);
			auto centrePoint = faceCentre + centreNorm * depth;
			pnts->addValue(centrePoint);

			//vectors
			OSG::Vec3f a = points[0] - points[2];
			OSG::Vec3f b = centrePoint - points[2];
			OSG::Vec3f c = points[1] - points[0];
			OSG::Vec3f d = centrePoint - points[0];			
			OSG::Vec3f e = points[2] - points[1];
			OSG::Vec3f f = centrePoint - points[1];

			//normals
			OSG::Vec3f normA = c.cross(d);
			OSG::Vec3f normB = a.cross(b);
			OSG::Vec3f normC = e.cross(f);
			OSG::Vec3f normD = c.cross(a);

			norms->push_back(normA);
			norms->push_back(normB);
			norms->push_back(normC);
			norms->push_back(normD);

			//colors
			colors->addValue(OSG::Color3f(1, 0, 0));
			colors->addValue(OSG::Color3f(1, 0, 0));
			colors->addValue(OSG::Color3f(1, 0, 0));
			colors->addValue(OSG::Color3f(1, 0, 0));

			//indices points
			int ip1 = indicesPtr;
			int ip2 = indicesPtr + 1;
			int ip3 = indicesPtr + 2;
			int ip4 = indicesPtr + 3;
			indicesPtr = indicesPtr + 4;
			
			//indicies
			indices->addValue(ip1);
			indices->addValue(ip2);
			indices->addValue(ip4);

			indices->addValue(ip3);
			indices->addValue(ip1);
			indices->addValue(ip4);

			indices->addValue(ip2);
			indices->addValue(ip3);
			indices->addValue(ip4);

			indices->addValue(ip1);
			indices->addValue(ip2);
			indices->addValue(ip3);

			//normals
			indicesnormpos->addValue(ip1);
			indicesnormpos->addValue(ip1);
			indicesnormpos->addValue(ip1);

			indicesnormpos->addValue(ip2);
			indicesnormpos->addValue(ip2);
			indicesnormpos->addValue(ip2);

			indicesnormpos->addValue(ip3);
			indicesnormpos->addValue(ip3);
			indicesnormpos->addValue(ip3);

			indicesnormpos->addValue(ip4);
			indicesnormpos->addValue(ip4);
			indicesnormpos->addValue(ip4);
		}			
	}

	lens->addValue(faceCount * (4 * 3));
	// Put it all together into a Geometry NodeCore.
	geo->setTypes(type);
	geo->setLengths(lens);

	// Points
	geo->setProperty(pnts, OSG::Geometry::PositionsIndex);
	geo->setIndex(indices, OSG::Geometry::PositionsIndex);
	// Normals
	geo->setProperty(norms, OSG::Geometry::NormalsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::NormalsIndex);
	// Colours
	geo->setProperty(colors, OSG::Geometry::ColorsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::ColorsIndex);

	geonode->setCore(geo);	

	return geonode;
}