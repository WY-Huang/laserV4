#ifndef SHOWIMGPCDDLG_H
#define SHOWIMGPCDDLG_H

#include <QDialog>
#include "opencv2/opencv.hpp"
#include "mypclfunction.h"

#define vtkRenderingCore_AUTOINIT 3(vtkRenderingOpenGL2, vtkInteractionStyle,vtkRenderingFreeType)

#include <vtkExtractSelection.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkCellPicker.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkScalarBarWidget.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkCubeAxesActor2D.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCubeAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCommand.h>
#include <vtkBorderWidget.h>
#include <vtkCamera.h>
#include <vtkBorderWidget.h>
#include <vtkCommand.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkVersion.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkAssembly.h>

#include <QVTKOpenGLNativeWidget.h>
#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/console/parse.h>
#include <pcl/range_image/range_image.h>
#include <pcl/visualization/common/float_image_utils.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/common.h>

#include <pcl/io/png_io.h>
#include <vtkColorSeries.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include "vtkPropPicker.h"
#include "vtkCylinderSource.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkHexahedron.h"
namespace Ui {
class showImgPcdDlg;
}

namespace {
// Catch mouse events
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
  static MouseInteractorStyle* New();

  MouseInteractorStyle()
  {
    selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    selectedActor = vtkSmartPointer<vtkActor>::New();
  }

  virtual void OnLeftButtonDown() override
  {
    vtkNew<vtkNamedColors> colors;

    // Get the location of the click (in window coordinates)
    int* pos = this->GetInteractor()->GetEventPosition();

    vtkNew<vtkCellPicker> picker;
    picker->SetTolerance(0.0005);

    // Pick from this location.
    picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

    double* worldPosition = picker->GetPickPosition();
    std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

    if (picker->GetCellId() != -1)
    {

      std::cout << "Pick position is: (" << worldPosition[0] << ", "
                << worldPosition[1] << ", " << worldPosition[2] << ")" << endl;

      vtkNew<vtkIdTypeArray> ids;
      ids->SetNumberOfComponents(1);
      ids->InsertNextValue(picker->GetCellId());

      vtkNew<vtkSelectionNode> selectionNode;
      selectionNode->SetFieldType(vtkSelectionNode::CELL);
      selectionNode->SetContentType(vtkSelectionNode::INDICES);
      selectionNode->SetSelectionList(ids);

      vtkNew<vtkSelection> selection;
      selection->AddNode(selectionNode);

      vtkNew<vtkExtractSelection> extractSelection;
      extractSelection->SetInputData(0, this->Data);
      extractSelection->SetInputData(1, selection);
      extractSelection->Update();

      // In selection
      vtkNew<vtkUnstructuredGrid> selected;
      selected->ShallowCopy(extractSelection->GetOutput());

      std::cout << "Number of points in the selection: "
                << selected->GetNumberOfPoints() << std::endl;
      std::cout << "Number of cells in the selection : "
                << selected->GetNumberOfCells() << std::endl;
      selectedMapper->SetInputData(selected);
      selectedActor->SetMapper(selectedMapper);
      selectedActor->GetProperty()->EdgeVisibilityOn();
      selectedActor->GetProperty()->SetColor(
          colors->GetColor3d("Tomato").GetData());

      selectedActor->GetProperty()->SetLineWidth(3);

      this->Interactor->GetRenderWindow()
          ->GetRenderers()
          ->GetFirstRenderer()
          ->AddActor(selectedActor);
    }
    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
  }

  vtkSmartPointer<vtkPolyData> Data;
  vtkSmartPointer<vtkDataSetMapper> selectedMapper;
  vtkSmartPointer<vtkActor> selectedActor;
};

vtkStandardNewMacro(MouseInteractorStyle);

} // namespace


class showImgPcdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit showImgPcdDlg(QWidget *parent = nullptr);
    ~showImgPcdDlg();

    pcl::PointCloud<pcl::PointXYZ>::Ptr pclclould;
    void showpoint(std::string filename);      //显示图像及点云

    // VTK显示点云
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    void vtk_init();

private:
    Ui::showImgPcdDlg *ui;
    MyPclFunction pclclass;//点云算法

    // vtk显示点云
    vtkSmartPointer<vtkEventQtSlotConnect> Connections;
};

#endif // SHOWIMGPCDDLG_H
