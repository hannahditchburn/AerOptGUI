#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include <memory>
#include <vector>
#include <QItemSelectionModel>
#include <QAbstractListModel>
#include "FileManipulation.h"

class Optimisation;
class DatabaseManager;

/**
 * @brief The OptimisationModel class provides a means of loading and interacting with several optimisations at once.
 */
class OptimisationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /**
     * @brief The Roles enum is used for differentiating output of the data method.
     * Qt::DisplayRole shuold be used to retrieve the label of an optimisation only.
     */
    enum Roles {
        Object = Qt::UserRole + 1
    };

    /**
     * @brief OptimisationModel Constructor method.
     * @param parent
     */
    OptimisationModel(QObject* parent = nullptr);

    /**
     * @brief addOptimisation Store a reference to an optimisation within the model.
     * @param optimisation The optimisation to store.
     * @return Index that loaded optimisation has been given in the context of the model.
     */
    QModelIndex addOptimisation(Optimisation *optimisation);

    /**
     * @brief rowCount Returns the number of loaded optimisations.
     * @param parent
     * @return The number of loaded optimisations.
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
     * @brief data Returns either an optimisation, or the label of an optimisation based on a given index.
     * Role should equal Roles::Object to retrieve the Optimisation object, or Qt::DisplayRole to retrieve the label only.
     * @param index Index of the optimisation within the model.
     * @param role Specifies what to retrieve: Qt::DisplayRole for label, Roles::Object for object.
     * @return Either the label or object for an optimisation, or otherwise just QVariant() if index is invalid.
     */
    QVariant data(const QModelIndex& index, int role) const override;

    /**
     * @brief removeRows Remove optimsations from the model.
     * @param row Initial optimisation index to removed
     * @param count number of optimisations to remove following row.
     * @param parent true iff successful, false if index row or count result in invalid indices.
     * @return
     */
    bool removeRows(int row, int count, const QModelIndex& parent) override;

    bool run(Optimisation* optimisation);

    void emitOptimisationFitnessChanged(Optimisation *optChanged);
    void emitOptimisationOutputChanged(Optimisation *optChanged);

    Optimisation *optimisation(uint index);

    /**
     * @brief revealFiles Opens the file explorer at the directory path for the given optimisation index.
     * @param index of optimisation in the model to load.
     */
    void revealFiles(int index);

    /**
     * @brief loadByInputFilePath
     * @param path
     * @return
     */
    QModelIndex loadByInputFilePath(QString path);

    /**
     * @brief isIndexValid Checks if a given index is valid within the optimisation model.
     * @param index Index value to check.
     * @return true iff index represents a valid optimisation loaded in the model.
     */
    bool isIndexValid(const QModelIndex& index) const;

signals:
    void optimisationFitnessChanged(int index);
    void optimisationOutputChanged(int index);

private:
    /**
     * @brief isIndexValid Checks if the given index is valud within the optimisation mode
     * Converts row into a QModelIndex and returns isIndexValid with converted data.
     * @param row Index value to check
     * @return true iff index represents a valid optimisation loaded in the model.
     */
    bool isIndexValid(int row);

    /**
     * @brief mOptimisations List of optimisations loaded within the model.
     */
    std::vector<Optimisation*> mOptimisations;
};

#endif // SIMULATIONMODEL_H
