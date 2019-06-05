#ifndef ICONITEMSMODEL_H
#define ICONITEMSMODEL_H

#include <QAbstractProxyModel>
#include "apk/resourceitemsmodel.h"
#include "apk/manifestscope.h"
#include "base/treenode.h"

class IconItemsModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    enum IconRow {
        ApplicationRow,
        ActivitiesRow,
        RowCount
    };

    enum IconColumn {
        CaptionColumn,
        PathColumn,
        TypeColumn,
        ColumnCount
    };

    enum IconType {
        Icon,
        RoundIcon,
        Banner
    };

    explicit IconItemsModel(QObject *parent = nullptr);
    ~IconItemsModel() override;

    void setSourceModel(ResourceItemsModel *sourceModel);
    ResourceItemsModel *sourceModel() const;

    QIcon getIcon() const;
    QIcon getIcon(const QModelIndex &index) const;
    QString getIconPath(const QModelIndex &index) const;
    QString getIconCaption(const QModelIndex &index) const;
    IconType getIconType(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    void sort(int column = 0, Qt::SortOrder order = Qt::AscendingOrder) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

signals:
    void ready() const;

private:
    struct IconNode : public TreeNode
    {
        IconNode(IconType type) : type(type) {}
        void addChild(TreeNode *node) = delete;
        const IconType type;
    };

    struct ActivityNode : public TreeNode
    {
        explicit ActivityNode(ManifestScope *scope) : scope(scope) {}
        void addChild(IconNode *node);
        const ManifestScope *scope;
    };

    bool appendIcon(const QPersistentModelIndex &index, ManifestScope *scope, IconType type = Icon);
    void onResourceAdded(const QModelIndex &index);
    void onResourceRemoved(const QModelIndex &index);
    void onResourceChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    const Project *apk() const;

    QHash<QPersistentModelIndex, IconNode *> sourceToProxyMap;
    QHash<IconNode *, QPersistentModelIndex> proxyToSourceMap;
    TreeNode *applicationNode;
    TreeNode *activitiesNode;
};

#endif // ICONITEMSMODEL_H
