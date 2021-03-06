#pragma once

#include <QWidget>
#include <QParallelAnimationGroup>

class QItemSelectionModel;
class QSortFilterProxyModel;
class QStyledItemDelegate;

namespace Ui { class KanbanColumnView; }

class KanbanColumnView : public QWidget
{
    Q_OBJECT

public:
	explicit KanbanColumnView(const QString& title, const QColor& columnColor, bool isCollapsed = false, QWidget *parent = nullptr);
	~KanbanColumnView();
    
	void setTitle(const QString& title);
	QString getTitle() const;

	void setColor(const QColor& color);
	QColor getColor() const;

	void setColumnSelection(bool isSelected) const;
	void setFilter(const QString& filter) const;
	void setModel(QSortFilterProxyModel* model) const;
	void setDelegate(QStyledItemDelegate* delegate) const;
	void selectKanbanItems(const QStringList& selectedKanbanItems);

	void deselectAllKanbanItems() const;
	bool isCollapsed() const;

signals:
	void kanbanCreated(const QString& columnName);
	void columnDeleted(const QString& columnName);
	void kanbanSelected(const QString& columnName, const QStringList& selectedText);

protected:
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	Ui::KanbanColumnView *ui;
	QParallelAnimationGroup mAnimation;
	QString mTitle;
	QColor mColor;
	int mExpandedWidth;
	int mCollapsedWidth;
	int mAnimationTime {250}; // milliseconds
	bool mIsCollapsed;
};
