#ifndef NEW_FLEET_FORM_H
#define NEW_FLEET_FORM_H
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include "Namespaces.h"


class NewFleetForm : public QWidget {
	Q_OBJECT
	public:
		NewFleetForm(QWidget* parent = nullptr);
		NewFleetForm(int fleetID, QWidget* parent = nullptr);

	private:
		QVBoxLayout* fleetLayout;
		QHBoxLayout* buttonsRow;
		QFormLayout* formLayout;

		QPushButton* saveButton{nullptr};
		QPushButton* cancelButton{nullptr};

		QLineEdit* fleetNumber{nullptr};
		QLineEdit* fleetLicensePlate{nullptr};
		QComboBox* fleetType{nullptr};
		QComboBox* fleetStatus{nullptr}; 
		QTextEdit* fleetStatusObservation{nullptr};
};
#endif
