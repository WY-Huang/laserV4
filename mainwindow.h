#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QThread>
#include <opencv2/opencv.hpp>
#include "rclcpp/rclcpp.hpp"
#include "my_params.h"
#include "mypclfunction.h"
//#include <vtkCubeAxesActor.h>
#include <QFileDialog>
#include <QtCore/QTextCodec>
#include <showimgpcddlg.h>
#include <getcurtime.h>
#include "cambuilddlg.h"

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
#include <vtkActor2DCollection.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ImgWindowShowThread;  // ???????????????????????????????????????

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    My_params *m_mcs;
    cv::Mat pImage;//??????????????????

    cambuilddlg * cambuild;

    std::vector<cv::Point3f> cv_line; //??????????????????
    MyPclFunction pclclass;//????????????

    void img_windowshow(bool b_show, QLabel *lab_show);      //????????????
    void showpoint(std::string filename);
    pcl::PointCloud<pcl::PointXYZ>::Ptr pclclould;
    volatile  bool b_int_show_record_finish;
    // VTK????????????
    vtkSmartPointer<vtkNamedColors> colors;
    vtkSmartPointer<vtkEventQtSlotConnect> slotConnector;
//    vtkSmartPointer<vtkPoints> points;
//    vtkSmartPointer<vtkCellArray> cells;
//    vtkSmartPointer<vtkPolyData> polydata;
//    vtkSmartPointer<vtkFloatArray> scalars;
//    vtkSmartPointer<vtkLookupTable> lut;
//    vtkSmartPointer<vtkPolyDataMapper> mapper;
//    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderer> renderer;
//    vtkSmartPointer<vtkScalarBarActor> scalarBar;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkAxesActor> axes_actor;
//    vtkSmartPointer<MouseInteractorStyle> style;
//    vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor;
    vtkSmartPointer<vtkPropPicker> propPicker;
    vtkSmartPointer<vtkRenderWindowInteractor> iren;
    vtkSmartPointer<vtkOrientationMarkerWidget> axes_actorWidget;
//    vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget;
    vtkPropPicker*  Picker;          // Pointer to the picker

    bool b_imgshow_thread;      //??????????????????
    bool stop_b_imgshow_thread;  //????????????????????????

    bool u8_save_data;     //????????????
    QString save_imgdata_cvimage(cv::Mat cv_image);  //??????opencv?????????
    QString save_pcldata_pclclould(pcl::PointCloud<pcl::PointXYZRGB>::Ptr pclclould);

    void start_deepimg();         //?????????????????????
    void stop_deepimg();          //??????????????????

    void vtk_init();

    volatile bool b_int_show_cvimage_inlab_finish;          //int_show_cvimage_inlab???????????????
    volatile bool b_init_show_pclclould_list_finish;          //init_show_pclclould_list???????????????

private slots:
    void int_show_cvimage_inlab(cv::Mat cv_image);// ????????????
    void init_show_pclclould_list(pcl::PointCloud<pcl::PointXYZRGB>::Ptr pclclould);      //???QVTKWidgetlib???????????????
//    void int_show_record(QString msg);
    void slot_timer_tragetor_clould();      //??????????????????????????????????????????

private:
    Ui::MainWindow *ui;
    showImgPcdDlg * showImgPcd;

    ImgWindowShowThread *imgshow_thread;

    void UpdateUi();  //???????????????????????????
    void InitSetEdit(); //?????????????????????

    QTimer *timer_tragetor_clould;      //??????????????????????????????

    void open_camer_modbus();
    void close_camer_modbus();

    void showupdata_tabWidget();

    // vtk????????????
    vtkSmartPointer<vtkEventQtSlotConnect> Connections;
};

// ??????????????????
class ImgWindowShowThread : public QThread
{
    Q_OBJECT
public:
    ImgWindowShowThread(MainWindow *statci_p);
    void Stop();
    void Lock();
    void unLock();
protected:
    void run();
private:
    MainWindow *_p;
    bool lock;
signals:
    // ???????????????
//    void Send_show_record(QString msg);
    void Send_show_cvimage_inlab(cv::Mat cv_image);       //????????????????????????
    void Send_show_pclclould_list(pcl::PointCloud<pcl::PointXYZRGB>::Ptr pclclould);      //????????????????????????
};

#endif // MAINWINDOW_H
