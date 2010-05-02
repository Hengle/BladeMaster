#include "AppPrivate.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
	//main window setup
	setWindowTitle(tr("BladeMaster"));

	//mdi area
	QMdiArea* mdiArea = new QMdiArea;
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdiArea);

	QTextEdit* textEdit = new QTextEdit;
	mdiArea->addSubWindow(textEdit);	

	//rendering window
	m_renderParentWidget = new QDockWidget(tr("Rendering"), this);
	m_renderParentWidget->setMinimumSize(1280,720);
	m_renderParentWidget->setMaximumSize(1280,720);
	m_renderParentWidget->setAllowedAreas(Qt::AllDockWidgetAreas);

	m_renderWidget = new QWidget(m_renderParentWidget);
	m_renderWidget->setMinimumSize(1280,720);
	m_renderWidget->setMaximumSize(1280,720);
	m_renderParentWidget->setWidget(m_renderWidget);
	
	addDockWidget(Qt::TopDockWidgetArea, m_renderParentWidget);
}

MainWindow::~MainWindow()
{

}
