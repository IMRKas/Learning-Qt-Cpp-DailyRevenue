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

namespace Goal {
	const QString Success = "Alcançada";
	const QString Failed = "Não Atingida";
};

namespace ShortfallReasons{
	const QString LackOfMaterial = "Falta de Material";
	const QString InterjourneyTeam = "Equipe de Interjornada";
	const QString EmergencyService = "Atendimento a Emergência";
	const QString PerformFailure = "Falha na Execução";
	const QString PlainningFailure = "Falha no Planejamento";
	const QString LowRevenueProgramming = "Programação a Menor";
	const QString DifficultiesDueToRain = "Dificuldades devido a Chuva";
	const QString LowRevenueJob = "Obra de baixa Produtividade";
	const QString DifficultiesDueToComplexity = "Dificuldades devido a complexidade da Obra";
	const QString MechanicalIssue = "Problemas Mecânicos";
	const QString TeamBreak = "Folga dada a Equipe";
};

namespace Sector {
	const QString Ccm = "CCM";
	const QString Warehouse = "ALMOX";
	const QString Sesmt = "SESMT";
	const QString Fleet = "FROTA";
	const QString Client = "EDP";
	const QString Team = "EQUIPE";
	const QString None = "N/A";
};



#endif

