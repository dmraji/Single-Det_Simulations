# ### VTK Rendering for spherical histogram
# 
# Create a 3D visualization to reder a 4$\pi$ image as a spherical histogram using VTK (pretty fast).
# We set it up by creating hexahedrons instead of cubes and then putting them into an unstructured grid.
# We then create one actor for the entire grid. Much better than using vtkCubeSource
import vtk as vtk
import matplotlib.pyplot as plt
import numpy as np
import healpy as hp

# Function to rotate vector about Y-axis
def RotateY(pos,rad):
    x_ = pos[2]*np.sin(rad)+pos[0]*np.cos(rad)
    y_ = pos[1]
    z_ = pos[2]*np.cos(rad)-pos[0]*np.sin(rad)
    return x_,y_,z_

# Function to rotate vector about Z-axis
def RotateZ(pos,rad):
    x_ = pos[0]*np.cos(rad)-pos[1]*np.sin(rad)
    y_ = pos[0]*np.sin(rad)+pos[1]*np.cos(rad)
    z_ = pos[2]
    return x_,y_,z_

# Function to translate vector by (x,y,z)
def Translate(pos,trans):
    x_ = pos[0]+trans[0]
    y_ = pos[1]+trans[1]
    z_ = pos[2]+trans[2]
    return x_,y_,z_

def SphericalHistogram(nside,image,hpindex=-1):

    # RBG array (from matplotlib)
    cmap = "jet"
    c_map = plt.get_cmap(cmap)
      
    # Create a lookup table for colormap
    lut = vtk.vtkLookupTable()
    lutNum = 256
    lut.SetNumberOfTableValues(lutNum)
    for i in range(0,lutNum):
        lut.SetTableValue(i,c_map(i)[0],c_map(i)[1],c_map(i)[2],1.0)
        
    # Create array of chars to hold colors for the hexahedrons
    cubeColors = vtk.vtkUnsignedCharArray()
    cubeColors.SetNumberOfComponents(3)
    cubeColors.SetName("color")
    
    # Create a renderer
    renderer = vtk.vtkRenderer()
    renderer.SetBackground(0.184,0.309,0.309)
    #renderer.SetBackground(1,1,1)
    
    # Define the nside for HEALPix
    #nside = 16
    
    # Define what we want to look at (image, sensitivity, etc.)
    #hpindex = 1190
    #image = np.dot(np.transpose(response), response)[hpindex,:]
    #image = recon[hpindex,:]
    #image = S
    
    # Scale the image to go from 0 to 1
    max = np.max(image)
    image /= max
    
    # Additional scaler to contrast bins more/less
    scaler = 1
    
    # Pull out the XYZ and theta/phi coordinates for HEALPix
    [x,y,z]     = hp.pix2vec(nside,range(0,hp.nside2npix(nside)))
    [phi,theta] = hp.pix2ang(nside,range(0,hp.nside2npix(nside)))
    


    # ====================================================================
    # Create all the scaled/colored boxes
    # ====================================================================
    
    # Define and fill an array to hold hex points
    points = vtk.vtkPoints()       
    for i in range(0,hp.nside2npix(nside)):
    
        z_ = image[i]*scaler    
        P0= [-0.02, -0.02, 0.0]
        P1= [+0.02, -0.02, 0.0]
        P2= [+0.02, +0.02, 0.0]
        P3= [-0.02, +0.02, 0.0]
        P4= [-0.02, -0.02,  z_]
        P5= [+0.02, -0.02,  z_]
        P6= [+0.02, +0.02,  z_]
        P7= [-0.02, +0.02,  z_]
        
        # Create the points (rotate Y by phi, rotate Z by theta, translate to correct position)
        points.InsertNextPoint(Translate(RotateZ(RotateY(P0,phi[i]),theta[i]),[x[i],y[i],z[i]]))
        points.InsertNextPoint(Translate(RotateZ(RotateY(P1,phi[i]),theta[i]),[x[i],y[i],z[i]]))
        points.InsertNextPoint(Translate(RotateZ(RotateY(P2,phi[i]),theta[i]),[x[i],y[i],z[i]]))
        points.InsertNextPoint(Translate(RotateZ(RotateY(P3,phi[i]),theta[i]),[x[i],y[i],z[i]]))
        points.InsertNextPoint(Translate(RotateZ(RotateY(P4,phi[i]),theta[i]),[x[i],y[i],z[i]]))
        points.InsertNextPoint(Translate(RotateZ(RotateY(P5,phi[i]),theta[i]),[x[i],y[i],z[i]]))
        points.InsertNextPoint(Translate(RotateZ(RotateY(P6,phi[i]),theta[i]),[x[i],y[i],z[i]]))
        points.InsertNextPoint(Translate(RotateZ(RotateY(P7,phi[i]),theta[i]),[x[i],y[i],z[i]]))
        
        # Set color for this index
        r,g,b = lut.GetTableValue(int(256*image[i]))[0:3]
        cubeColors.InsertNextTupleValue((int(255*r), int(255*g),int(255*b)))
        
    
    # Create and fill an array with the hexahedrons
    theHex = []
    for i in range(0,hp.nside2npix(nside)):
        theHex.append(vtk.vtkHexahedron())
        theHex[i].GetPointIds().SetNumberOfIds(8)
        for j in range(0,8):
            theHex[i].GetPointIds().SetId(j,j+(i*8)) # map internal ID (0-7) to global ID (0-npix)
    
    # Create an unstructured grid for the hexahedrons
    theGrid = vtk.vtkUnstructuredGrid()
    theGrid.Allocate(hp.nside2npix(nside))
    for i in range(0,hp.nside2npix(nside)):
        theGrid.InsertNextCell(theHex[i].GetCellType(), theHex[i].GetPointIds())
    
    # Set the points
    theGrid.SetPoints(points)
    
    # Add the colors as scalars
    theGrid.GetCellData().AddArray(cubeColors);
    theGrid.GetCellData().SetActiveScalars("color")
    
    # Create a mapper and actor 
    theHexMapper = vtk.vtkDataSetMapper()
    theHexMapper.SetInputData(theGrid)
    actor = vtk.vtkActor()
    actor.SetMapper(theHexMapper)
    
    # Add to the render
    renderer.AddActor(actor);
    
    # ====================================================================
    
    
          
    # Create/place a sphere behind boxes
    sphere = vtk.vtkSphereSource()
    sphere.SetRadius(1)
    sphere.SetThetaResolution(75)
    sphere.SetPhiResolution(75)
    sphere_mapper = vtk.vtkPolyDataMapper()
    sphere_mapper.SetInputConnection(sphere.GetOutputPort())
    sphere_actor = vtk.vtkActor()
    sphere_actor.SetMapper(sphere_mapper)
    sphere_actor.GetProperty().SetColor(c_map(0)[0],c_map(0)[1],c_map(0)[2])
    renderer.AddActor(sphere_actor) 
    
    if hpindex != 1:
        # Create a line pointing to the true source pixel
        phi,theta = hp.pix2ang(nside,hpindex)
        r_ = (1+image[i]*scaler)*5
        x_ = r_ * np.cos(theta)*np.sin(phi)
        y_ = r_ * np.sin(theta)*np.sin(phi)
        z_ = r_ * np.cos(phi)
        sourceline = vtk.vtkLineSource()
        sourceline.SetPoint1(0,0,0)
        sourceline.SetPoint2(x_,y_,z_)
        sourceline.SetResolution(30)
        linemapper = vtk.vtkPolyDataMapper()
        linemapper.SetInputConnection(sourceline.GetOutputPort())
        lineactor = vtk.vtkActor()
        lineactor.SetMapper(linemapper)
        lineactor.GetProperty().SetLineWidth(1)
        lineactor.GetProperty().SetColor(0,0,0)
        renderer.AddActor(lineactor)
    
    # Create and place a title
    titleActor = vtk.vtkTextActor()
    titleActor.SetInput ("PRISM Spherical Histogram");
    titleActor.SetPosition2 (10, 40);
    titleActor.GetTextProperty().SetFontSize (20);
    titleActor.GetTextProperty().SetColor (1.0, 0.0, 0.0);
    renderer.AddActor2D(titleActor);
    
    # Create a render window
    render_window = vtk.vtkRenderWindow()
    render_window.SetSize(800, 600)
    render_window.AddRenderer(renderer)
    
    # Create an interactor
    interactor = vtk.vtkRenderWindowInteractor()
    interactor.SetRenderWindow(render_window)
    style = vtk.vtkInteractorStyleTrackballCamera() 
    interactor.SetInteractorStyle(style)

    # Add axes to the render
    axes = vtk.vtkAxesActor()
    axeslength = (1+image[i]*scaler)*5
    axes.SetTotalLength(axeslength,axeslength,axeslength)
    axes.SetNormalizedTipLength(0,0,0)
    axes.SetConeRadius(0)
    renderer.AddActor(axes)
    
    # Create a colormap legend
    color_bar = vtk.vtkScalarBarWidget()
    color_bar.SetInteractor(interactor)
    color_bar.SetDefaultRenderer(renderer)
    color_bar.SetCurrentRenderer(renderer)
    color_bar.SetEnabled(True)
    color_bar.GetScalarBarActor().SetLookupTable(lut)
    color_bar.GetScalarBarActor().SetNumberOfLabels(5)
    color_bar.GetRepresentation().SetOrientation(1)  # 0 = Horizontal, 1 = Vertical
    color_bar.GetRepresentation().GetPositionCoordinate().SetValue(0.05,0.05)    # relative to window 
    color_bar.GetRepresentation().GetPosition2Coordinate().SetValue(0.053,0.45)  # relative to window 
    
    # Initialize the interactor and start the rendering loop
    interactor.Initialize()
    render_window.Render()
    interactor.Start()