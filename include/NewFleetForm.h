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

// --------------- Combo Box --------------- //
// ComboBox items
namespace FleetStatus {
	const QString Broken = "Quebrado";
	const QString Running = "Rodando";
	const QString InWorkshop = "Oficina";
	const QString Backup = "Reserva";
};

namespace FleetType {
	const QString Truck = "Caminhão";
	const QString PickupTruck = "Caminhonete";
	const QString Motorcycle = "Moto";
	const QString LightVehicle = "Veículo Leve";
};

// Data added to DB
namespace DbStatus {
	const QString running = "RODANDO";
	const QString broken = "QUEBRADO";
	const QString inWorkshop = "OFICINA";
	const QString backup = "RESERVA";
};

namespace DbFleet {
	const QString truck = "CAMINHÃO";
	const QString pickupTruck = "CAMINHONETE";
	const QString motorcycle = "MOTO";
	const QString lightVehicle = "VEÍCULO LEVE";
};

// ---------------- END -----------------------//


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
