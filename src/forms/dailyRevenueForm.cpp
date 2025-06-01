#include "DailyRevenueForm.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QLocale>
#include <QDebug>

DailyRevenueForm::DailyRevenueForm(QWidget* parent) : QWidget(parent){
	setWindowTitle("ADICIONAR PRODUÇÃO DO DIA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts Def
	dailyLayout = new QVBoxLayout(this);
	dailyForm = new QFormLayout;
	buttonsRow = new QHBoxLayout;

	dateTeamRow = new QHBoxLayout;
	projectRevenueRow = new QHBoxLayout;
	reasonRow = new QHBoxLayout;



	// Form Settings
	revenueDate = new QDateEdit(); 
	teams = new QComboBox(this);
	projectNumber = new QLineEdit(this); projectNumber->setMaxLength(12);
	dailyRevenue = new QLineEdit(this); dailyRevenue->setMaxLength(16); dailyRevenue->setPlaceholderText("Número Apenas");
	goalAchieved = new QLineEdit(this); goalAchieved->setEnabled(false);
	diffRevenue = new QLineEdit(this); diffRevenue->setEnabled(false);
	notAchievedReason = new QComboBox(this);
	sectorResponsible= new QComboBox(this);

	// Labels
	dateLabel = new QLabel("Data");
	teamLabel = new QLabel("Equipe");
	goalLabel = new QLabel("Meta");
	projectLabel = new QLabel("OV/Nota");
	reasonLabel = new QLabel("Motivo");
	diffLabel = new QLabel("Dif.");


	// Buttons
	saveButton = new QPushButton("Salvar",this);
	cancelButton = new QPushButton("Cancelar", this);

	// Setting Layouts
	buttonsRow->addWidget(saveButton);
	buttonsRow->addWidget(cancelButton);

		dateTeamRow->addWidget(revenueDate);
		dateTeamRow->addWidget(projectLabel);
		dateTeamRow->addWidget(projectNumber);
		dateTeamRow->addWidget(teamLabel);
		dateTeamRow->addWidget(teams);

		projectRevenueRow->addWidget(dailyRevenue);
		projectRevenueRow->addWidget(diffLabel);
		projectRevenueRow->addWidget(diffRevenue);
		projectRevenueRow->addWidget(goalLabel);
		projectRevenueRow->addWidget(goalAchieved);

		reasonRow->addWidget(sectorResponsible);
		reasonRow->addWidget(reasonLabel);
		reasonRow->addWidget(notAchievedReason);

	dailyForm->addRow("Data", dateTeamRow);
	dailyForm->addRow("Produção", projectRevenueRow);
	dailyForm->addRow("Responsável", reasonRow);

	dailyLayout->addLayout(dailyForm);
	dailyLayout->addLayout(buttonsRow);
	
	// Reasons Combo Box settings
	notAchievedReason->addItem("");
	QSqlQuery getReasons;
	getReasons.prepare("SELECT id, reason FROM reasons ORDER BY reason ASC;");
	if(getReasons.exec()){
		while(getReasons.next()){
			notAchievedReason->addItem(getReasons.value(1).toString(),getReasons.value(0).toInt());
		}
	}	
	// Responsible Combo Box
	sectorResponsible->addItem("");
	QSqlQuery getResponsible;
	getResponsible.prepare("SELECT id, responsible_name FROM responsible;");
	if(getResponsible.exec()){
		while(getResponsible.next()){
			sectorResponsible->addItem(getResponsible.value(1).toString(),getResponsible.value(0).toInt());
		}
	}

	// Team ComboBox
	QSqlQuery getTeams;
	getTeams.prepare("SELECT id, team_name FROM teams ORDER BY team_name;");
	if(getTeams.exec()){
		while(getTeams.next()){
			teams->addItem(getTeams.value(1).toString(),getTeams.value(0).toInt());
		}
	} else {
		QMessageBox::critical(this, "ERRO", "Falha ao buscar dados da tabela teams:\n" + getTeams.lastError().text());
	}

	// DateEdit Settings
	QDate currentDate = QDate::currentDate();
	revenueDate->setDate(currentDate);
	revenueDate->setCalendarPopup(true); 
	revenueDate->setDisplayFormat("dd/MM/yyyy");
	revenueDate->setLocale(QLocale(QLocale::Portuguese, QLocale::Brazil));
	//Regex and formating
	QLocale brazil(QLocale::Portuguese, QLocale::Brazil);
	QRegularExpression regex("\\d+");
	dailyRevenue->setValidator(new QRegularExpressionValidator(regex, dailyRevenue));
	projectNumber->setValidator(new QRegularExpressionValidator(regex, projectNumber));

	// Connects Settings
	connect(dailyRevenue, &QLineEdit::textChanged, [=](const QString &newText){
			static bool updating = false;
			if(updating) return;
			
			QString validInput = newText;
			validInput.remove(QRegularExpression("[^\\d]"));

			if(validInput.isEmpty()){
				dailyRevenue->clear();
				return;
			}

			qlonglong cents = validInput.toLongLong();
			double realInput = cents / 100.0;

			QString brazilianCurrency = brazil.toCurrencyString(realInput);

			updating = true;
			dailyRevenue->setText(brazilianCurrency);
			double teamGoal = getTeamDailyRevenueGoal(teams->currentData().toInt()) / 100.0;
			if(realInput - teamGoal < 0){
				goalAchieved->setText(Goal::Failed);
			} else {
				goalAchieved->setText(Goal::Success);
			}
			diffRevenue->setText(brazil.toCurrencyString(realInput - teamGoal));
			updating = false;
	});

	connect(teams, &QComboBox::currentTextChanged, this, [=](){
			static bool updating = false;
			if(updating) return;
			
			QString validInput = dailyRevenue->text();
			validInput.remove(QRegularExpression("[^\\d]"));

			if(validInput.isEmpty()){
				dailyRevenue->clear();
				return;
			}

			qlonglong cents = validInput.toLongLong();
			double realInput = cents / 100.0;
			QString brazilianCurrency = brazil.toCurrencyString(realInput);

			updating = true;
			double teamGoal = getTeamDailyRevenueGoal(teams->currentData().toInt()) / 100.0;
			if(realInput - teamGoal < 0){
				goalAchieved->setText(Goal::Failed);
			} else {
				goalAchieved->setText(Goal::Success);
			}
			diffRevenue->setText(brazil.toCurrencyString(realInput - teamGoal));
			updating = false;
	});

	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			if((projectNumber->text().isEmpty()) || (projectNumber->text().toInt() <= 0 || dailyRevenue->text().isEmpty())){
				QMessageBox::warning(this, "ATENÇÃO", "Insira um número de OV/NOTA e/ou para Produção");
			} else {
				if(goalAchieved->text() == Goal::Failed && (notAchievedReason->currentData().isNull() || sectorResponsible->currentData().isNull())) {
						QMessageBox::warning(this, "ATENÇÃO", "Se a Meta do Dia não foi alcançada, deve haver um motivo");
				} else {
						QSqlQuery saveRevenue;
						saveRevenue.prepare("INSERT INTO daily_revenue (project_number, date, id_team, total_daily_revenue, revenue_diff,"
											"goal_achieved, id_sector, id_reason) VALUES (?,?,?,?,?,?,?,?);");
						saveRevenue.addBindValue(projectNumber->text().toInt());
						saveRevenue.addBindValue(revenueDate->date().toString(Qt::ISODate));
						saveRevenue.addBindValue(teams->currentData().toInt());
						saveRevenue.addBindValue(static_cast<int>(dailyRevenue->text().remove(QRegularExpression("[^\\d]")).toLongLong()));
						saveRevenue.addBindValue(static_cast<int>(diffRevenue->text().remove(QRegularExpression("[^\\d-]")).toLongLong()));
						saveRevenue.addBindValue(goalAchieved->text());
						saveRevenue.addBindValue((sectorResponsible->currentData().isNull()) ? QVariant(QVariant::Int) : QVariant(sectorResponsible->currentData().toInt()));
							saveRevenue.addBindValue((notAchievedReason->currentData().isNull()) ? QVariant(QVariant::Int) : QVariant(notAchievedReason->currentData().toInt()));

						if(!saveRevenue.exec()){
							QMessageBox::critical(this, "ERRO", "Falha ao adicionar dados no Banco de Dados:\n" + saveRevenue.lastError().text());
							return;
							} else {
							QMessageBox::information(this, "SUCESSO", "Produção Diária Adicionada");
							this->close();
						}

				}
			}
	});


}

DailyRevenueForm::DailyRevenueForm(int dayID, QWidget* parent) : QWidget(parent){
	setWindowTitle("ADICIONAR PRODUÇÃO DO DIA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts Def
	dailyLayout = new QVBoxLayout(this);
	dailyForm = new QFormLayout;
	buttonsRow = new QHBoxLayout;

	dateTeamRow = new QHBoxLayout;
	projectRevenueRow = new QHBoxLayout;
	reasonRow = new QHBoxLayout;



	// Form Settings
	revenueDate = new QDateEdit(); 
	teams = new QComboBox(this);
	projectNumber = new QLineEdit(this); projectNumber->setMaxLength(12);
	dailyRevenue = new QLineEdit(this); dailyRevenue->setMaxLength(16); dailyRevenue->setPlaceholderText("Número Apenas");
	goalAchieved = new QLineEdit(this); goalAchieved->setEnabled(false);
	diffRevenue = new QLineEdit(this); diffRevenue->setEnabled(false);
	notAchievedReason = new QComboBox(this);
	sectorResponsible= new QComboBox(this);

	// Labels
	dateLabel = new QLabel("Data");
	teamLabel = new QLabel("Equipe");
	goalLabel = new QLabel("Meta");
	projectLabel = new QLabel("OV/Nota");
	reasonLabel = new QLabel("Motivo");
	diffLabel = new QLabel("Dif.");


	// Buttons
	saveButton = new QPushButton("Salvar",this);
	cancelButton = new QPushButton("Cancelar", this);

	// Setting Layouts
	buttonsRow->addWidget(saveButton);
	buttonsRow->addWidget(cancelButton);

		dateTeamRow->addWidget(revenueDate);
		dateTeamRow->addWidget(projectLabel);
		dateTeamRow->addWidget(projectNumber);
		dateTeamRow->addWidget(teamLabel);
		dateTeamRow->addWidget(teams);

		projectRevenueRow->addWidget(dailyRevenue);
		projectRevenueRow->addWidget(diffLabel);
		projectRevenueRow->addWidget(diffRevenue);
		projectRevenueRow->addWidget(goalLabel);
		projectRevenueRow->addWidget(goalAchieved);

		reasonRow->addWidget(sectorResponsible);
		reasonRow->addWidget(reasonLabel);
		reasonRow->addWidget(notAchievedReason);

	dailyForm->addRow("Data", dateTeamRow);
	dailyForm->addRow("Produção", projectRevenueRow);
	dailyForm->addRow("Responsável", reasonRow);

	dailyLayout->addLayout(dailyForm);
	dailyLayout->addLayout(buttonsRow);
	
	// Reasons Combo Box settings
	notAchievedReason->addItem("");
	QSqlQuery getReasons;
	getReasons.prepare("SELECT id, reason FROM reasons ORDER BY reason ASC;");
	if(getReasons.exec()){
		while(getReasons.next()){
			notAchievedReason->addItem(getReasons.value(1).toString(),getReasons.value(0).toInt());
		}
	}
	
	// Responsible Combo Box
	sectorResponsible->addItem("");
	QSqlQuery getResponsible;
	getResponsible.prepare("SELECT id, responsible_name FROM responsible;");
	if(getResponsible.exec()){
		while(getResponsible.next()){
			sectorResponsible->addItem(getResponsible.value(1).toString(),getResponsible.value(0).toInt());
		}
	}

	// Team ComboBox
	QSqlQuery getTeams;
	getTeams.prepare("SELECT id, team_name FROM teams ORDER BY team_name;");
	if(getTeams.exec()){
		while(getTeams.next()){
			teams->addItem(getTeams.value(1).toString(),getTeams.value(0).toInt());
		}
	}

	// DateEdit Settings
	revenueDate->setCalendarPopup(true); 
	revenueDate->setDisplayFormat("dd/MM/yyyy");
	revenueDate->setLocale(QLocale(QLocale::Portuguese, QLocale::Brazil));
	//Regex and formating
	QLocale brazil(QLocale::Portuguese, QLocale::Brazil);
	QRegularExpression regex("\\d+");
	dailyRevenue->setValidator(new QRegularExpressionValidator(regex, dailyRevenue));
	projectNumber->setValidator(new QRegularExpressionValidator(regex, projectNumber));

	// Get data from Selected Row
	QSqlQuery getData;
	getData.prepare("SELECT daily_revenue.date, daily_revenue.project_number, teams.team_name,"
						 "printf('R$ %.2f', daily_revenue.total_daily_revenue/100.0), printf('R$ %.2f', revenue_diff/100.0), "
						 "goal_achieved, responsible.responsible_name, "
						 "reasons.reason FROM daily_revenue " 
						 "LEFT JOIN teams ON daily_revenue.id_team = teams.id "
						 "LEFT JOIN responsible ON daily_revenue.id_sector = responsible.id "
						 "LEFT JOIN reasons ON daily_revenue.id_reason = reasons.id "
						 "WHERE daily_revenue.id = ?;");
	getData.addBindValue(dayID);
	if(getData.exec() && getData.next()){
		revenueDate->setDate(QDate::fromString(getData.value(0).toString(), Qt::ISODate));
		projectNumber->setText(getData.value(1).toString());
		teams->setCurrentText(getData.value(2).toString());
		dailyRevenue->setText(getData.value(3).toString());
		diffRevenue->setText(getData.value(4).toString());
		goalAchieved->setText(getData.value(5).toString());
		sectorResponsible->setCurrentText(getData.value(6).toString());
		notAchievedReason->setCurrentText(getData.value(7).toString());

	} else {
		QMessageBox::critical(this, "ERRO", "Erro ao buscar dado da linha: \n" + getData.lastError().text());
		return;
	}

	// Connects Settings
	connect(dailyRevenue, &QLineEdit::textChanged, [=](const QString &newText){
			static bool updating = false;
			if(updating) return;
			
			QString validInput = newText;
			validInput.remove(QRegularExpression("[^\\d]"));

			if(validInput.isEmpty()){
				dailyRevenue->clear();
				return;
			}

			qlonglong cents = validInput.toLongLong();
			double realInput = cents / 100.0;
			QString brazilianCurrency = brazil.toCurrencyString(realInput);

			updating = true;
			dailyRevenue->setText(brazilianCurrency);
			double teamGoal = getTeamDailyRevenueGoal(teams->currentData().toInt()) / 100.0;
			if(realInput - teamGoal < 0){
				goalAchieved->setText(Goal::Failed);
			} else {
				goalAchieved->setText(Goal::Success);
			}
			diffRevenue->setText(brazil.toCurrencyString(realInput - teamGoal));
			updating = false;
	});

	connect(teams, &QComboBox::currentTextChanged, this, [=](){
			static bool updating = false;
			if(updating) return;
			
			QString validInput = dailyRevenue->text();
			validInput.remove(QRegularExpression("[^\\d]"));

			if(validInput.isEmpty()){
				dailyRevenue->clear();
				return;
			}

			qlonglong cents = validInput.toLongLong();
			double realInput = cents / 100.0;
			QString brazilianCurrency = brazil.toCurrencyString(realInput);

			updating = true;
			double teamGoal = getTeamDailyRevenueGoal(teams->currentData().toInt()) / 100.0;
			if(realInput - teamGoal < 0){
				goalAchieved->setText(Goal::Failed);
			} else {
				goalAchieved->setText(Goal::Success);
			}
			diffRevenue->setText(brazil.toCurrencyString(realInput - teamGoal));
			updating = false;
	});


	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			if((projectNumber->text().isEmpty()) || (projectNumber->text().toInt() <= 0 && dailyRevenue->text().isEmpty())){
				QMessageBox::warning(this, "ATENÇÃO", "Insira um número de OV/NOTA");
			} else {
				if(goalAchieved->text() == Goal::Failed && (notAchievedReason->currentData().isNull() || sectorResponsible->currentData().isNull())) {
						QMessageBox::warning(this, "ATENÇÃO", "Se a Meta do Dia não foi alcançada, deve haver um motivo");
				} else {
						QSqlQuery saveRevenue;
						saveRevenue.prepare("UPDATE daily_revenue SET project_number = ?, date = ?, id_team = ?, total_daily_revenue = ?, "
								"revenue_diff = ?, goal_achieved = ?, id_sector = ?, id_reason = ? WHERE daily_revenue.id = ?;");
						saveRevenue.addBindValue(projectNumber->text().toInt());
						saveRevenue.addBindValue(revenueDate->date().toString(Qt::ISODate));
						saveRevenue.addBindValue(teams->currentData().toInt());
						saveRevenue.addBindValue(static_cast<int>(dailyRevenue->text().remove(QRegularExpression("[^\\d]")).toLongLong()));
						saveRevenue.addBindValue(static_cast<int>(diffRevenue->text().remove(QRegularExpression("[^\\d-]")).toLongLong()));
						saveRevenue.addBindValue(goalAchieved->text());
						saveRevenue.addBindValue((sectorResponsible->currentData().isNull()) ? QVariant(QVariant::Int) : QVariant(sectorResponsible->currentData().toInt()));
						saveRevenue.addBindValue((notAchievedReason->currentData().isNull()) ? QVariant(QVariant::Int) : QVariant(notAchievedReason->currentData().toInt()));
						saveRevenue.addBindValue(dayID);

						if(!saveRevenue.exec()){
							QMessageBox::critical(this, "ERRO", "Falha ao atualizar dados no Banco de Dados:\n" + saveRevenue.lastError().text());
							return;
							} else {
							QMessageBox::information(this, "SUCESSO", "Produção Diária Atualizada");
							this->close();
						}

				}
			}
	});


}


int DailyRevenueForm::getTeamDailyRevenueGoal(int team_id){
	QSqlQuery getTeamGoal;
	getTeamGoal.prepare("SELECT team_daily_revenue_goal FROM teams WHERE id = ?;");
	getTeamGoal.addBindValue(team_id);
	if(getTeamGoal.exec() && getTeamGoal.next()){
		return getTeamGoal.value(0).toInt();
	}

	return 0;
}

		

