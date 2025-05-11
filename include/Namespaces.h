#ifndef NAMESPACE_DEFINITIONS_H
#define NAMESPACE_DEFINITIONS_H

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

namespace TeamStatus{
	const QString Working = "Trabalhando";
	const QString InactiveTeam = "Parada";
	const QString DefunctTeam = "Desmobilizada";
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


#endif

