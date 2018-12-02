#include "ui_KanbanColumnView.h"
#include "KanbanColumnView.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>
#include <QGraphicsOpacityEffect>

KanbanColumnView::KanbanColumnView(const QString& title, const QColor& columnColor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanbanColumnView)
{
    ui->setupUi(this);
    ui->mButtonSpoiler->setCheckable(true);
    ui->mLabelTitle->setText(title);

    ui->mLabelTitleVertical->setText(title);
    ui->mLabelTitleVertical->setVisible(false);
	ui->mLabelTitleVertical->setBackgroudColor(columnColor);

	setupListView();

    setMinimumWidth(width());
    setMaximumWidth(width());

    mAnimation.addAnimation(new QPropertyAnimation(this, "minimumWidth"));
    static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setDuration(mAnimationTime);
    static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setEasingCurve(QEasingCurve::InOutBack);
    
    mAnimation.addAnimation(new QPropertyAnimation(this, "maximumWidth"));
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setDuration(mAnimationTime);
    static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setEasingCurve(QEasingCurve::InOutBack);	

	QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
	ui->mLabelTitleVertical->setGraphicsEffect(opacity);
	mAnimation.addAnimation(new QPropertyAnimation(opacity,"opacity"));
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(2))->setDuration(mAnimationTime);
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(2))->setEasingCurve(QEasingCurve::Linear);

	QGraphicsOpacityEffect *opacity2 = new QGraphicsOpacityEffect(this);
	ui->mLabelTitle->setGraphicsEffect(opacity2);
	mAnimation.addAnimation(new QPropertyAnimation(opacity2,"opacity"));
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(3))->setDuration(mAnimationTime);
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(3))->setEasingCurve(QEasingCurve::Linear);

	QGraphicsOpacityEffect *opacity3 = new QGraphicsOpacityEffect(this);
	ui->mListView->setGraphicsEffect(opacity3);
	mAnimation.addAnimation(new QPropertyAnimation(opacity3,"opacity"));
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(4))->setDuration(mAnimationTime);
	static_cast<QPropertyAnimation*>(mAnimation.animationAt(4))->setEasingCurve(QEasingCurve::Linear);

    connect(ui->mButtonSpoiler, &QToolButton::clicked, [this](bool isToggled)
    {
        mIsCollapsed = isToggled;
        const auto newSize = mIsCollapsed ? mMinSize : mMaxSize;
        ui->mButtonSpoiler->setArrowType(mIsCollapsed ? Qt::RightArrow : Qt::LeftArrow);

        //static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setStartValue(width());
        static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setStartValue(minimumWidth());
        static_cast<QPropertyAnimation*>(mAnimation.animationAt(0))->setEndValue(newSize);

        //static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setStartValue(width());
        static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setStartValue(maximumWidth());
        static_cast<QPropertyAnimation*>(mAnimation.animationAt(1))->setEndValue(newSize);

		static_cast<QPropertyAnimation*>(mAnimation.animationAt(2))->setStartValue(!mIsCollapsed);
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(2))->setEndValue(mIsCollapsed);
		
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(3))->setStartValue(mIsCollapsed);
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(3))->setEndValue(!mIsCollapsed);

        static_cast<QPropertyAnimation*>(mAnimation.animationAt(4))->setStartValue(mIsCollapsed);
		static_cast<QPropertyAnimation*>(mAnimation.animationAt(4))->setEndValue(!mIsCollapsed);
    	
    	mAnimation.start();
    });

    connect(&mAnimation, &QParallelAnimationGroup::finished, [this]()
    {
        setMinimumWidth(width());
        setMaximumWidth(width());

        ui->mListView->setVisible(!mIsCollapsed);
        ui->mLabelTitle->setVisible(!mIsCollapsed);        
        ui->mButtonOptions->setVisible(!mIsCollapsed);
        ui->mLabelTitleVertical->setVisible(mIsCollapsed);
    });

    connect(ui->mButtonOptions, &QToolButton::clicked, [this](bool)
    {
        emit columnDeleted(getTitle());
    });	

	connect(ui->mButtonAdd, &QPushButton::clicked, [this](bool)
	{
		emit kanbanCreated(getTitle());
	});
}

KanbanColumnView::~KanbanColumnView()
{
    qDebug() << "Deleted " << getTitle();
    delete ui;
}

void KanbanColumnView::setupListView() const
{
	// TODO: check if all this settings are needed

	ui->mListView->setSpacing(5);
	ui->mListView->setResizeMode(QListView::Adjust);
	ui->mListView->setFlow(QListView::TopToBottom);
	ui->mListView->setUniformItemSizes(false);
	ui->mListView->setMovement(QListView::Free);
	ui->mListView->setMouseTracking(true);
	ui->mListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->mListView->setDragEnabled(true);
	ui->mListView->setAcceptDrops(true);
	ui->mListView->setDropIndicatorShown(true);
	ui->mListView->setDefaultDropAction(Qt::MoveAction);
	ui->mListView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    ui->mListView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void KanbanColumnView::setModel(QSortFilterProxyModel* model) const
{
	ui->mListView->setModel(model);
}

void KanbanColumnView::setDelegate(QStyledItemDelegate* delegate) const
{
	ui->mListView->setItemDelegate(delegate);
}

QString KanbanColumnView::getTitle() const
{
    return ui->mLabelTitle->text();
}

void KanbanColumnView::mouseDoubleClickEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	ui->mButtonSpoiler->click();
}
