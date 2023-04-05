#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#define ADMIN_USER "airadmin" //define tou username kai tou password tou admin
#define ADMIN_PASS "Air123"   //wste na mporoun efkola na allaxtoun sto mellon

/*dhmiourgia domhs pou periexei ta arxika, ton foro aerodromiou kai to kostos pthshs enos proorismou*/
struct flightData {
	char airport_code[4];
	int airport_tax;
	int flight_cost;
};
typedef struct flightData FlightData;

/*dhmiourgia domhs gia ta stoixeia ths odou enos passenger*/
struct address {
	char street_name[50];
	int number;
	char city[50];
	int zip_code;
};
typedef struct address Address;

/*dhmiourgia domhs gia ta stoixeia enos passenger (periexei kai ena melos typou struct address me ta stoixeia ths odou)*/
struct passenger {
	char first_name[50];
	char last_name[50];
	Address add;
	char username[50];
	char password[50];
	char flights[3][4][20];
};
typedef struct passenger Passenger;

/*prototyping twn synarthsewn pou emfanizoun ta diafora menu tou programmatos*/
void main_menu();
void admin_menu(FlightData data[]);
void user_menu(Passenger user[], FlightData data[]);
void login_menu(Passenger user[], int *user_number, FlightData data[]);

/*prototyping twn synarthsewn pou epiteloun tis diafores leitourgies tou programmatos*/
void set_prices(FlightData data[]);
void signup_user(Passenger user[], int *userPtr);
void login_user(Passenger user[], int *userPtr, FlightData data[]);
void view_user(Passenger user[], int *user_number);
void modify_user(Passenger user[], int *user_number);
void calculate_cost(Passenger user[], int *user_number, FlightData data[], int counter);
void payment(Passenger user[], int *user_number, int paid_trip[][3]);
void most_expensive(Passenger user[], int *user_number, int paid_trip[][3]);

int main(int argc, char *argv[]) {
	main_menu(); //klhsh ths synarthshs pou emfanizei to kyrio menu tou programmatos
	return 0;
}

/*H synarthsh emfanizei to kyrio menu tou programmatos*/
void main_menu() {
	static FlightData airport[10]; //dhlwsh pinaka domhs flightData gia ta 10 diaforetika aerodromia
	const char airport_initials[10][4] = {"ATH", "ROM", "MAD", "LON", "BER", "AMS", "NYW", "BEI", "CHI", "SYD"}; //dhlwsh 2d pinaka pou periexei ta arxika kathe aerodromiou
	int i;
	for (i = 0; i <= 9; i++) {
		strcpy(airport[i].airport_code, airport_initials[i]); //perasma sto string airport_code ths domhs airport twn arxikwn kathe aerodromiou
	}
	static Passenger pass[20]; //dhlwsh pinaka domhs passenger gia tous 20 diaforetikous users pou mporoun na ftiaksoun logiariasmo
	printf("Welcome to main menu. Type:");
	printf("\n\"user\" to enter as a user");
	printf("\n\"admin\" to enter as an admin");
	printf("\n\"0\" to exit the program\n");
	char input[20];
	static int flag = 0; //flag pou xrhsimopoieitai gia na doume an o admin exei eisagei tis times
	printf("Input: ");
	scanf(" %s", input);
	/*elegxos ths eisodou tou xrhsth gia thn eisagwgh sto analogo menu*/
	if (strcmp(input, "admin") == 0) {
		system("cls");
		/*an o xrhsths epileksei "admin" tote zhtate epanalipthka h eisagwgh twn credentials mexri na dothoun ta swsta*/
		while (1) {
			char username[20];
			char password[20];
			printf("Please enter your credentials\n");
			printf("Username: ");
			scanf(" %s", username);
			printf("Password: ");
			scanf(" %s", password);
			if ((strcmp(username, ADMIN_USER) == 0) && (strcmp(password, ADMIN_PASS) == 0)) {
				printf("\n\nLogin Successful!");
				sleep(2);
				system("cls");
				break;
			}
			else {
				printf("\n\nLogin Failed!");
				sleep(2);
				system("cls");
			}
		}
		flag = 1; //to flag pou deixnei an exei mpei o admin gia na thesei tis times ginetai 1
		admin_menu(airport); //klhsh ths synarthshs pou emfanizei to menu tou admin. Sth synarthsh pername ton pinaka me ta stoixeia twn aerodromiwn
	}
	else if (strcmp(input, "user") == 0) {
		system("cls");
		/*Elegxos gia to an exei mpei o admin na thesei tis times. An oxi tote emfanizetai katallhlo mhnyma alliws o xrhsths mpainei sto user menu*/
		if (flag == 0) {
			printf("Cannot use the system at the moment. Please try again later!");
			sleep(3);
			system("cls");
			main_menu();
		}
		else {
			user_menu(pass, airport); //klhsh ths synarthshs pou emfanizei to menu tou user
		}	
	}
	else if (strcmp(input, "0")) {
		exit(0); //termatismos tou programmatos me thn xrhsh ths synarthshs exit
	}
}

/*H synarthsh emfanizei to menu tou admin*/
void admin_menu(FlightData data[]) {
	static int admin_flag = 0; //flag pou deixnei an o admin exei epileksei na kanei set tis times. Xrhsimopoieitai gia na apotrepsei ton admin na kanei logout prin thesei tis times
	printf("Welcome to the admin menu. Select: ");
	printf("\na. Set Prices");
	printf("\nb. Log Out");
	printf("\nType: ");
	char selection;
	scanf(" %c", &selection);
	if (selection == 'a') {
		system("cls");
		admin_flag = 1; //allagh ths timhs tou flag se 1 prin ginei klhsh ths synarthshs set_prices
		set_prices(data);
	}
	else if (selection == 'b') {
		system("cls");
		//elegxos gia to an exoun oritsthei oi times apo ton admin kai apotroph eksodou apo to menu an den exoun oristhei
		if (admin_flag == 0) {
			printf("You have not set the prices yet! Please select \"Set Prices\" from the admin menu.");
			sleep(3);
			system("cls");
			admin_menu(data);
		}
		else {
			main_menu(); //klhsh ths synarthshs pou emfanizei to arxiko menu tou programmatos
		}
	}
}

/*H synarthsh epitrepei ston admin na orisei to kostos tou forou kai ths pthshs gia ta aerodromia kathws kai na ta allaksei*/
void set_prices(FlightData data[]) {
	static int cost_check = 0; //flag pou xrhsimopoieitai gia kseroume an o xrhsths exei orisei tis times kai na emfanizetai to katallhlo menu epilogwn
	int i;
	/*Elegxos gia to an exoun oristhei oi times. An nai tote emfanizetai ena menu epilogwn me olous tous proorismous wste na epileksei
	o admin poies times thelei na allaksei. An oxi tote prepei na eisagei times gia olous tous proorismous*/
	if (cost_check == 0) {
		printf("The tax and flight costs have not been set. Please set the costs for each destination\n\n");
		for (i = 0; i <= 9; i++) {
			printf("Destination: %s\n", data[i].airport_code);
			printf("Airport Tax: ");
			scanf("%d", &data[i].airport_tax);
			printf("Flight Cost: ");
			scanf("%d", &data[i].flight_cost);
			system("cls");
		}
		cost_check = 1;
		printf("The prices have been successfuly set!");
		sleep(2);
		system("cls");
	}
	else {
		/*Synexhs emfanish tou menu epilogwn me tous proorismous mexri o xrhsths na epileksei na vgei (arithmos 0 sto menu)*/
		while (1) {
			printf("Please select a destination to change the tax or flight cost:\n");
			int i;
			/*Emfanish menu me olous tous proorismous kai tous antistixous arithmous epiloghs tou kathenos*/
			for (i=1; i<=10; i++) {
				printf("%d. %s\n", i, data[i-1].airport_code);
			}
			printf("0. Exit\n");
			int selection;
			printf("Select: ");
			scanf("%d", &selection);
			if (selection == 0) {
				system("cls");
				break;
			}
			else {
				system("cls");
				printf("%s\n", data[selection-1].airport_code);
				printf("Please select:");
				printf("\na. Airport Tax");
				printf("\nb. Flight Cost");
				char type;
				printf("\nType: ");
				scanf(" %c", &type);
				system("cls");
				if (type == 'a') {
					printf("Current Price: %d", data[selection-1].airport_tax);
					printf("\nEnter New Price: ");
					scanf("%d", &data[selection-1].airport_tax);
				}
				else {
					printf("Current Price: %d", data[selection-1].flight_cost);
					printf("\nEnter New Price: ");
					scanf("%d", &data[selection-1].flight_cost);
				}
				printf("\n\nPrice changed successfully!");
				sleep(2);
				system("cls");
				}
			}
		}
	admin_menu(data); //klhsh ths synarthshs admin_menu gia epistrofh sto menu tou admin	
	}
	
/*H synarthsh emfanizei to menu tou user (dexetai ws orismata ton pinaka me ta stoixeia twn diaforwn users kai ton pinaka me ta stoixeia gia 
tous proorismous, kathws tha xreiastoun apo thn synarthsh calculate_cost)*/
void user_menu(Passenger user[], FlightData data[]) {
	static int user_count = 0; //static akeraia metavlhth pou metraei tous xrhstes pou exoun eggrafei sto systhma wste na apotrepetai h eggrafh panw apo 20 atomwn
	/*Emfanish tou menu epilogwn*/
	printf("Welcome to the user menu. Select:");
	printf("\na. Register");
	printf("\nb. Login");
	printf("\nc. Logout");
	printf("\nType: ");
	char select;
	scanf(" %c", &select);
	if (select == 'a') {
		/*Elegxos gia to an o arithmos twn eggegrammenwn xrhstwn exei ftasei tous 20 kai emfanisei antistoixou mhnymatos sthn periptwsh auth*/
		if (user_count == 20) {
			printf("\n\nMaximum user limit reached! Cannot register at the current time!");
			sleep(3);
			system("cls");
			user_menu(user, data);
		}
		else {	
			system("cls");
			signup_user(user, &user_count); //klhsh ths synarthshs gia thn eggrafh tou xrhsth
			user_menu(user, data); //klhsh ths idias ths synarthshs gia na emfanistei ksana to menu tou user
		}
	}
	else if (select == 'b') {
		/*Elegxos gia to an yparxoun users pou exoun eggrafei sto systhma. An oxi tote emfanizetai katallhlo mhnyma, alliws kaleitai h synarthsh gia login tou xrhsth*/
		if (user_count == 0) {
			printf("\n\nNo accounts exist in the database!");
			sleep(3);
			system("cls");
			user_menu(user, data);
		}
		else {
			system("cls");
			login_user(user, &user_count, data);
		}
	}
	else if (select == 'c') {
		system("cls");
		main_menu(); //epistrofh sto arxiko menu 
	}
}

/*H synarthsh epitrepei ston xrhsth na kanei eggrafh sto systhma. Dexetai ws orismata twn pinaka pou tha periexei ta stoixeia twn passengers
kathws kai ena pointer pou deixnei ston arithmo tou ekastote xrhsth(o arithmos vgainei apo to user_count ths synarthshs user_menu, pou auksanetai kata 1 kathe fora
pou enas neos xrhsths epilegei na kanei register sto systhma)*/
void signup_user(Passenger user[], int *userPtr) {
	printf("Welcome! To create an account please enter the following information:\n");
	printf("First Name: ");
	scanf(" %s", user[*userPtr].first_name);
	printf("Last Name: ");
	scanf(" %s", user[*userPtr].last_name);
	printf("Address:\n");
	printf(" Street Name: ");
	scanf(" %[^\n]s", user[*userPtr].add.street_name);
	printf(" Street Number: ");
	scanf("%d", &user[*userPtr].add.number);
	printf(" City: ");
	scanf(" %[^\n]s", user[*userPtr].add.city);
	printf(" Zip Code: ");
	scanf("%d", &user[*userPtr].add.zip_code);
	printf("Password: ");
	scanf(" %s", user[*userPtr].password);
	int i;
	/*Metatroph tou epithetou pou edwse o user se peza grammata wste na dhmiourghthei to username*/
	for (i=0; i<=50; i++) {
		if (user[*userPtr].last_name[i] == '\0') {
			break;
		}
		user[*userPtr].username[i] = tolower(user[*userPtr].last_name[i]);
	}
	strcat(user[*userPtr].username, "456"); //prosthikh tou 456 sto telos tou username wste na oristhei to teliko username
	printf("\n\nAccount created successfully! Your username is: \"%s\"", user[*userPtr].username); //Emfanish mhnymatos epityxias kathws kai tou username tou xrhsth
	(*userPtr)++; //aukshsh tou arithmou tou xrhsth kata ena mesw tou deikth pou deixnei sto user_count
	sleep(5);
	system("cls");
}

/*H synarthsh epitrepei ston xrhsth na kanei login ston logariasmo pou exei dhmiourghsei sto systhma */
void login_user(Passenger user[], int *userPtr, FlightData data[]) {
	printf("Please enter your credentials\n");
	char user_input[50];
	char pass_input[50];
	printf("Username: ");
	scanf(" %s", user_input);
	printf("Password: ");
	scanf(" %s", pass_input);
	int found = 0; //flag gia to an exei vrethei to username tou xrhsth sto systhma
	int i;
	/*Elegxos gia to an yparxei to username pou exei eisagei o xrhsths sto systhma*/
	for (i=0; i<= *userPtr; i++) {
		if (strcmp(user_input, user[i].username) == 0) {
			found = 1;
			break;
		}
	}
	/*An den vrethei to username tote emfanizetai error*/
	if (found == 0) {
		printf("\n\nError! Account with this username does not exist!");
		sleep(3);
		system("cls");
		login_user(user, userPtr, data);
	}
	else {
		/*An vrethei to username sto systhma tote eksetazetai an to password einai swsto kai emfanizetai katallhlo mhnyma*/
		if (strcmp(pass_input, user[i].password) == 0) {
			printf("\n\nLogin Successful!");
			sleep(3);
			system("cls");
			login_menu(user, &i, data); //klhsh tou login menu tou xrhsth
		}
		else {
			printf("\n\nPassword Incorrect!");
			sleep(3);
			system("cls");
			login_user(user, userPtr, data);
		}
	}	
}

/*H synarthsh emfanizei to login menu tou xrhsth kai dexetai ws orismata ton pinaka me ta stoixeia twn xrhstwn, ena pointer pou deixnei ston arithmo
tou user(to i ths epanalhpshs ths synarthshs login menu, kathws otan vrethei to username h epanalhpsh stamataei, ara to i einai o arithmos tou user), 
kai ton pinaka me ta stoixeia twn aerodromiwn*/
void login_menu(Passenger user[], int *user_number, FlightData data[]) {
	static int calculate_flag[20] = {0}; //arxikopoihsh me 0 enos pinaka me flag gia to an exei ypologistei to kostos twn taxidiwn analoga me ton xrhsth
	static int payment_flag[20] = {0}; //antistoixa me to prohgoumeno flag, auto deixnei an o xrhsths exei plhrwsei gia estw kai 1 taksidh
	static int paid_trip[20][3] = {0}; //autos o 2d pinakas exei 3 flags gia kathe xrhsth pou deixnoun poia apo ta 3 taksidia tou kathenos exoun plhrwthei
	printf("Welcome user \"%s\"! You can select any of the following options:", user[*user_number].username);
	printf("\n1. View (to view your account info)");
	printf("\n2. Modify (to modify your account info)");
	printf("\n3. Calculate (to calculate the cost of your trips)");
	/*Elegxos gia to an o user exei ypologisei to kostos twn pthsewn tou, wste na emfanizontai oi epiloges ths plhrwmhs kai tou pio akrivou taksidiou
	sto menu h oxi*/
	if (calculate_flag[*user_number] != 0) {
		printf("\n4. Payment (to pay for your trips)");
		printf("\n5. Most Expensive (to view the data of your most expensive trip)");
		printf("\n6. Logout");
	}
	else {
		printf("\n4. Logout");
	}
	int choice;
	printf("\nType: ");
	scanf(" %d", &choice);
	if (choice == 1) {
		system("cls");
		view_user(user, user_number);
		login_menu(user, user_number, data);
	}
	else if (choice == 2) {
		system("cls");
		modify_user(user, user_number);
		login_menu(user, user_number, data);
	}
	else if (choice == 3) {
		system("cls");
		int i;
		/*Epanalhpsh gia klhsh 3 fores ths synarthshs calculate_cost*/
		for (i=0; i<=2; i++) {
			calculate_cost(user, user_number, data, i); //sthn calculate_cost dinetai ws orisma to i (o arithmos ths epanalhpshs), wste na kseroume poia pthsh afora kathe fora h synarthsh
		}
		calculate_flag[*user_number] = 1; //allagh ths timhs tou calculate flag gia ton sygkekrimeno xrhsth 
		login_menu(user, user_number, data);
	}
	else if (choice == 4) {
		/*Elegxos gia to an to calculate_flag tou xrhsth einai 0. Se auth thn periptwsh to noumero 4 sto menu tha antistoixei sto logout kai oxi sto
		payment*/
		if (calculate_flag[*user_number] == 0) {
			system("cls");
			user_menu(user, data);
		}
		else {
			payment_flag[*user_number] = 1; //allagh ths timhs tou payment flag tou xrhsth se 1
			system("cls");
			payment(user, user_number, paid_trip);
			login_menu(user, user_number, data);
		}
	}
	else if (choice == 5) {
		/*Elegxos gia to an o xrhsths exei plhrwsei gia kapoio taksidi mesw ths payment_flag. Se periptwsh pou den exei emfanizetai mhnyma, afou 
		den yparxei plhrwmeno taksidi wste na emfanistei to pio akribo*/
		if (payment_flag[*user_number] == 1) {
			system("cls");
			most_expensive(user, user_number, paid_trip);
			login_menu(user, user_number, data);
		}
		else {
			printf("\n\nYou have not paid for any of your trips! Please select \"Payment\" to do so!");
			sleep(3);
			system("cls");
			login_menu(user, user_number, data);
		}
	}
	else if (choice == 6) {
		system("cls");
		user_menu(user, data);
	}
}

/*Mesw auths ths synarthshs o xrhsths mporei na dei ta stoixeia tou logariasmou tou. Xrhsimopoieitai to pointer to opoio deixnei ston arithmo tou 
user, wste na vrethoun ta stoixeia tou ston pinaka twn domwn typou passenger*/
void view_user(Passenger user[], int *user_number) {
	printf("Welcome user \"%s\"! Your personal information are:", user[*user_number].username);
	printf("\nFirst Name: %s", user[*user_number].first_name);
	printf("\nLast Name: %s", user[*user_number].last_name);
	printf("\nAddress:");
	printf("\n Street Name: %s", user[*user_number].add.street_name);
	printf("\n Street Number: %d", user[*user_number].add.number);
	printf("\n City: %s", user[*user_number].add.city);
	printf("\n Zip Code: %d", user[*user_number].add.zip_code);
	printf("\nPassword: %s", user[*user_number].password);
	printf("\n\n");
	system("pause");
	system("cls");
}

/*H synarthsh epitrepei ston xrhsth na allaksei ta stoixeia tou logariasmou tou (mono auta pou sxetizontai me thn odo tou)*/
void modify_user(Passenger user[], int *user_number) {
	printf("You can now modify your personal information!");
	printf("\nInformation modification is only available for your address!");
	printf("\nYour current address info is: ");
	printf("\n Street Name: %s", user[*user_number].add.street_name);
	printf("\n Street Number: %d", user[*user_number].add.number);
	printf("\n City: %s", user[*user_number].add.city);
	printf("\n Zip Code: %d", user[*user_number].add.zip_code);
	printf("\nPress");
	printf("\n a. To modify your address info");
	printf("\n b. To go back to the login menu");
	char selection;
	printf("\nType: ");
	scanf(" %c", &selection);
	if (selection == 'a') {
		system("cls");
		/*Eisagwgh apo ton xrhsth twn newn dedomenwn pou aforoun thn odo tou*/
		printf("Enter your new information\n");
		printf("Street Name: ");
		scanf(" %[^\n]s", user[*user_number].add.street_name);
		printf("Street Number: ");
		scanf("%d", &user[*user_number].add.number);
		printf("City: ");
		scanf(" %[^\n]s", user[*user_number].add.city);
		printf("Zip Code: ");
		scanf("%d", &user[*user_number].add.zip_code);
		printf("\n\nAccount information edited successfully!");
		sleep(3);
		system("cls");
	}
	else {
		system("cls");
	}
}

/*H synarthsh auth emfanizei tou diathesimous proorismous kai zhtaei apo ton xrhsth na epileksei */
void calculate_cost(Passenger user[], int *user_number, FlightData data[], int counter) {
	printf("To book a trip please select a destination.");
	printf("\nAvailable destinations: ");
	printf("\n Rome");
	printf("\n Madrid");
	printf("\n London");
	printf("\n Berlin");
	printf("\n Amsterdam");
	printf("\n New York");
	printf("\n Beijing");
	printf("\n Chicago");
	printf("\n Sydney");
	printf("\nSelect: ");
	char destination[50];
	scanf(" %[^\n]s", destination);
	int i; //xrhsh mias metavlhths pou pairnei timh analoga me ton proorismo wste na mporoume na ton vroume ston pinaka me ta dedomena gia ta aerodromia (h timh antiproswpeuei thn thesh tou aerodromiou ston pinaka)
	if (strcmp(destination, "Rome") == 0) {
		i = 1;
	}
	else if (strcmp(destination, "Madrid") == 0) {
		i = 2;
	}
	else if (strcmp(destination, "London") == 0) {
		i = 3;
	}
	else if (strcmp(destination, "Berlin") == 0) {
		i = 4;
	}
	else if (strcmp(destination, "Amsterdam") == 0) {
		i = 5;
	}
	else if (strcmp(destination, "New York") == 0) {
		i = 6;
	}
	else if (strcmp(destination, "Beijing") == 0) {
		i = 7;
	}
	else if (strcmp(destination, "Chicago") == 0) {
		i = 8;
	}
	else if (strcmp(destination, "Sydney") == 0) {
		i = 9;
	}
	/*xrhshmopoiwntas synarthseis ths vivliothhkhs time.h kai ths synarthshs itoa vazoume se strings thn hmera kai ton mhna kai sthn synexeia ta enonnoume se ena string*/
	time_t t = time(NULL);
	struct tm *local;
	local = localtime(&t);
	char day[3];
	char month[3];
	char date[5];
	itoa(local->tm_mday, day, 10);
	itoa((local->tm_mon)+1, month, 10);
	/*Ginetai elegxos gia to an h hmera h o mhnas einai monopshfioi arithmoi. Sthn periptwsh auth prostithetai ena mhdeniko mprosta apo ton arithmo 
	wste na exei to swsto format (p.x. 01 anti gia 1) kai sthn synexeia na emfanizetai swsta ston arithmo pthshs (p.x. ATHAMS1501 kai oxi ATHAMS151)*/
	if (local->tm_mday <= 9) {
		day[1] = day[0];
		day[0] = '0';
	}
	if (((local->tm_mon)+1) <= 9) {
		month[1] = month[0];
		month[0] = '0';
	}
	strcpy(date, day);
	strcat(date, month);
	int j;
	/*Dhmiourgia twn dyo arithmwn pthshs xrhsimopoiwntas to string me to date kathws kai ta airport codes apo ton pinaka data*/
	for (j=0; j<=1; j++) {
		if (j == 0) {
			strcpy(user[*user_number].flights[counter][j], "ATH");
			strcat(user[*user_number].flights[counter][j], data[i].airport_code);
		}
		else {
			strcpy(user[*user_number].flights[counter][j], data[i].airport_code);
			strcat(user[*user_number].flights[counter][j], "ATH");
		}
		strcat(user[*user_number].flights[counter][j], date);
	}
	/*Ypologismos tou telikou kostous twn pthsewn mesw stoixeiwn apo ton pinaka data*/
	int final_cost;
	int outbound_flight_cost;
	int return_flight_cost;
	outbound_flight_cost = data[i].flight_cost + data[i].airport_tax;
	return_flight_cost = data[0].flight_cost + data[0].airport_tax;
	final_cost = outbound_flight_cost + return_flight_cost;
	char cost[20];
	itoa(final_cost, cost, 10);
	strcpy(user[*user_number].flights[counter][2], cost);
	strcpy(user[*user_number].flights[counter][3], data[i].airport_code);
	system("cls");
}

/*H synarthshs epitrepei ston xrhsth na plhrwsei gia ta taksidia tou*/
void payment(Passenger user[], int *user_number, int paid_trip[][3]) {
	/*Emfanish antistoixou mhnymatos an o xrhsths exei plhrwsei gia ola ta taksidia tou*/
	if ((paid_trip[*user_number][0] == 1) && (paid_trip[*user_number][1] == 1) && (paid_trip[*user_number][2] == 1)) {
		printf("You have paid for all of your trips!");
		sleep(3);
		system("cls");
	}
	else {
		/*Emfanish twn taksidiwn pou den exoun plhrwthei xrhsimopoiwntas ton 2d pinaka paid_trip*/
		printf("You have not paid for the following trips:\n");
		int i;
		for (i=0; i<3; i++) {
			if (paid_trip[*user_number][i] == 0) {
				printf(" %s: %s EUR\n", user[*user_number].flights[i][3], user[*user_number].flights[i][2]);
			}
		}
		printf("To pay for a trip please enter the destination: ");
		char dest[4];
		scanf(" %s", dest);
		for (i=0; i<3; i++) {
			if (strcmp(dest, user[*user_number].flights[i][3]) == 0) {
				break;
			}
		}
		system("cls");
		/*Synexhs epanalhpsh pou zhtaei apo ton xrhsth na plhktrologhsei to kostos tou taksidiou, mexri na dwsei to swsto kostos*/
		while (1) {
			printf("The cost of the trip to %s is %s EUR", user[*user_number].flights[i][3], user[*user_number].flights[i][2]);
			printf("\nPlease enter the cost to confirm the payment: ");
			int cost = atoi(user[*user_number].flights[i][2]);
			int cost_input;
			scanf("%d", &cost_input);
			if (cost == cost_input) {
				printf("\n\nPayment Successful!");
				paid_trip[*user_number][i] = 1;
				sleep(3);
				system("cls");
				break;
			}
			else {
				printf("\n\nPayment Failed!");
				sleep(3);
				system("cls");
			}
		}
	}
}

/*H synarthsh emfanizei to pio akrivo apo ta taksidia pou exei plhrwsei o user*/
void most_expensive(Passenger user[], int *user_number, int paid_trip[][3]) {
	int costs[3];
	int max = 0;
	int max_position = 0;
	int i;
	/*Euresh ths theshs tou pio akrivou taksidiou wste na kseroume se poia thesi ths prwths diastashs tou 3d pinaka flights na psaksoume*/
	for (i=0; i<3; i++) {
		costs[i] = atoi(user[*user_number].flights[i][2]);
		if ((costs[i] > max) && (paid_trip[*user_number][i] == 1)) {
			max = costs[i];
			max_position = i;
		}
	}
	printf("The most expensive out of your paid flights is the flight to %s with a cost of %d EUR", user[*user_number].flights[max_position][3], max);
	sleep(5);
	system("cls");
}
