# 3d-printing-spike-generator
An algorithm which allows you to take a polygonal mesh and add a new surface giving it depth. The new depth is given below by adding a "spike" to each face giving it better strength when 3d printing it. 

## Basic usage
```c++
OSG::NodeRefPtr geo = OSG::makeTeapot(2.0f, 1.0f); //create basic geometry
auto spikes = drawSpikes(distance, depth, geo);

scene->addChild(file);
scene->addChild(spikes);
```
