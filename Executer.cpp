#include <iostream>
#include <cstdlib>

#include <iomanip> // to have access to std::setprecision (unsigned int)
// to print a double number to the console I use os << std::fixed << std::setprecision (number_of_digits_after_the_comma) << double_number
// chatGPT recommended me this way to cut double numbers

#include "Executer.h"


Executer::Executer (Uber& uber) : uberSystem (uber) {}

const String ENTER_NUMBER = "\nEnter a number, corresponding to the choice you wish to make!\n";

const String TYPE_NUMBER = "Type a number, corresponding to the one of the options above that you wish to execute: ";
const String INVALID_INPUT = "Invalid input!\n";


void Executer::goToMainScreen () {
    using std::cout;
    currentUser.clear ();
    cout << ENTER_NUMBER;
    cout << "1. Register\n";
    cout << "2. Log in\n";
    cout << "3. Quit\n";
    ConsoleInterfaceForSystem interface;
    int userInput;
    interface.enterNumberOfChoice (userInput, TYPE_NUMBER, INVALID_INPUT, 3);
    switch (userInput) {
        case 1 : uberSystem.Register (); break;
        case 2 : currentUser = uberSystem.logIn (); break;
        case 3 : return; break;
    }
    // if register, then currentUser.getUserType () will not be changed and will be zero
    // if logging in, then currentUser.getUserType () will be 1 if logged as client; 2 if logged as driver or 3 if logging in was unsuccessfull
    static const unsigned short CLIENT = 1;
    static const unsigned short DRIVER = 2;
    static const unsigned short NO_SUCH_USER = 3;
    switch (currentUser.getUserType ()) {
        case 0 : goToMainScreen ();; break;
        case CLIENT : goToClientScreen ();; break;
        case DRIVER : goToDriverScreen ();; break;
        case NO_SUCH_USER : std::cout << "\nInvalid username or password!\n"; goToMainScreen (); break;
    }
}

void Executer::goToClientScreen () {
    using std::cout;
    cout << ENTER_NUMBER;
    cout << "1. Read messages.\n";
    static const unsigned int NEXT_CHOICE = 2;
    unsigned int additionalChoice = 0;

    UberClient& currentClient = uberSystem.arrayOfClients[currentUser.getIndexInArray ()];
    if (!currentClient.currentOrder.isInitialised () || currentClient.currentOrder -> getStatus () == OrderStatus::cancelled
                                                     || currentClient.currentOrder -> getStatus () == OrderStatus::concluded) {
        cout << "2. Make an order.\n";
        ++additionalChoice;
    }
    else if (currentClient.currentOrder -> getStatus () == OrderStatus::registered) {
        cout << "2. Cancel your order.\n";
        ++additionalChoice;
    }
    else if (currentClient.currentOrder -> getStatus () == OrderStatus::recieved) {
        cout << "2. Cancel your order.\n";
        cout << "3. Check your order.\n";
        additionalChoice += 2;
    }
    else if (currentClient.currentOrder -> getStatus () == OrderStatus::completed) {
        cout << "2. Pay for your order.\n";
        cout << "3. Check order.\n";
        additionalChoice += 2;
    }
    else if (currentClient.currentOrder -> getStatus () == OrderStatus::paid) {
        cout << "2. Check order.\n";
        ++additionalChoice;
    }
    cout << NEXT_CHOICE + additionalChoice << ". Add money.\n";
    cout << NEXT_CHOICE + additionalChoice + 1 << ". See money.\n";
    cout << NEXT_CHOICE + additionalChoice + 2 << ". Log out.\n";

    ConsoleInterfaceForSystem interface;
    int userInput;
    interface.enterNumberOfChoice (userInput, TYPE_NUMBER, INVALID_INPUT, 6);

    cout << "\n";

    if (!currentClient.currentOrder.isInitialised () || currentClient.currentOrder -> getStatus () == OrderStatus::cancelled
                                                     || currentClient.currentOrder -> getStatus () == OrderStatus::registered
                                                     || currentClient.currentOrder -> getStatus () == OrderStatus::paid
                                                     || currentClient.currentOrder -> getStatus () == OrderStatus::concluded) {
        switch (userInput) {
            case 1 : currentClient.readMesseges (); goToClientScreen (); break;
            case 2 :
                if (!currentClient.currentOrder.isInitialised () || currentClient.currentOrder -> getStatus () == OrderStatus::cancelled
                                                                 || currentClient.currentOrder -> getStatus () == OrderStatus::concluded) {
                    Order orderMade = currentClient.makeAnOrder ();
                    uberSystem.arrayOfActiveOrders.pushBack (orderMade);
                    MySmartPtr<Order, MAX_NUMBER_OF_ORDER_HOLDERS> ptr = MySmartPtr<Order, MAX_NUMBER_OF_ORDER_HOLDERS> (new Order (orderMade));
                    currentClient.currentOrder = ptr;
                    int indexOfClosestDriver = uberSystem.findClosetDriver (orderMade);
                    if (indexOfClosestDriver != -1) {
                        uberSystem.arrayOfDrivers[indexOfClosestDriver].currentOrder = ptr;
                    }
                    goToClientScreen ();
                }
                else if (currentClient.currentOrder -> getStatus () == OrderStatus::registered) {
                    currentClient.cancelOrder ();
                    currentClient.currentOrder -> setStatus (OrderStatus::cancelled);
                    goToClientScreen ();
                }
                else if (currentClient.currentOrder -> getStatus () == OrderStatus::paid) {
                    currentClient.checkOrder ();
                    goToClientScreen ();
                }
                break;
            case 3 : currentClient.addMoney (); goToClientScreen (); break;
            case 4 : cout << "You have " << std::fixed << std::setprecision (2) << currentClient.getMoney () << " " << CURRENCY << ".\n"; goToClientScreen (); break;
            case 5 : goToMainScreen (); break;
            case 6 : goToClientScreen (); break;
        }
    }
    else if (currentClient.currentOrder -> getStatus () == OrderStatus::recieved || currentClient.currentOrder -> getStatus () == OrderStatus::completed) {
        switch (userInput) {
            case 1 : currentClient.readMesseges (); goToClientScreen (); break;
            case 2 : 
                if (currentClient.currentOrder -> getStatus () == OrderStatus::recieved) {
                    currentClient.cancelOrder (); 
                    currentClient.currentOrder -> setStatus (OrderStatus::cancelled); 
                    goToClientScreen ();
                }
                else if (currentClient.currentOrder -> getStatus () == OrderStatus::completed) {
                    if (currentClient.availableMoney < currentClient.currentOrder -> getCost ()) {
                        cout << "You do not have enough avaible money to pay!\n";
                        goToClientScreen ();
                        break;
                    }
                    double moneyPaid = currentClient.payOrder ();
                    currentClient.currentOrder -> setMoneyPaid (moneyPaid);
                    unsigned short ratigGiven = currentClient.rate ();
                    currentClient.currentOrder -> setRatingGiven (ratigGiven);
                    size_t indexOfDriver = uberSystem.findIndexOfDriver ((currentClient.currentOrder -> getDriverData ()).getUsername ());
                    uberSystem.arrayOfDrivers[indexOfDriver].ratingsRecieved.pushBack (ratigGiven);
                    currentClient.currentOrder -> driverData.setRating (uberSystem.arrayOfDrivers[indexOfDriver].getRating ());
                    currentClient.currentOrder -> setStatus (OrderStatus::paid);
                    currentClient.availableMoney -= moneyPaid;
                    goToClientScreen ();
                }
                break;
            case 3 : currentClient.checkOrder (); goToClientScreen (); break;
            case 4 : currentClient.addMoney (); goToClientScreen (); break;
            case 5 : cout << "You have " << std::fixed << std::setprecision (2) << currentClient.getMoney () << " " << CURRENCY << ".\n"; goToClientScreen (); break;
            case 6 : goToMainScreen (); break;
        }
    }
    else {
        throw std::runtime_error ("Unknown error on client screen!");
    }
}

void Executer::goToDriverScreen () {
    using std::cout;
    cout << ENTER_NUMBER;
    static const unsigned int NEXT_CHOICE = 2;
    unsigned int additionalChoice = 0;
    cout << "1. Read messages.\n";
    UberTaxiDriver& currentDriver = uberSystem.arrayOfDrivers[currentUser.getIndexInArray ()];
    if (!currentDriver.currentOrder.isInitialised () || currentDriver.currentOrder -> getStatus () == OrderStatus::cancelled
                                                     || currentDriver.currentOrder -> getStatus () == OrderStatus::concluded) {
        cout << "2. Change position.\n";
        ++additionalChoice;
    }
    else if (currentDriver.currentOrder -> getStatus () == OrderStatus::registered) {
        cout << "2. Accept order.\n";
        cout << "3. Decline order\n";
        additionalChoice += 2;
    }
    else if (currentDriver.currentOrder -> getStatus () == OrderStatus::recieved) {
        cout << "2. Finish order.\n";
        ++additionalChoice;
    }
    else if (currentDriver.currentOrder -> getStatus () == OrderStatus::paid) {
        cout << "2. Accept payment.\n";
        ++additionalChoice;
    }
    cout << NEXT_CHOICE + additionalChoice << ". See income.\n";
    cout << NEXT_CHOICE + additionalChoice + 1 << ". See rating.\n";
    cout << NEXT_CHOICE + additionalChoice + 2 << ". Log out.\n";

    ConsoleInterfaceForSystem interface;
    int userInput;
    interface.enterNumberOfChoice (userInput, TYPE_NUMBER, INVALID_INPUT, 6);

    cout << "\n";

    if (!currentDriver.currentOrder.isInitialised () || currentDriver.currentOrder -> getStatus () == OrderStatus::cancelled
                                                     || currentDriver.currentOrder -> getStatus () == OrderStatus::concluded
                                                     || currentDriver.currentOrder -> getStatus () == OrderStatus::recieved
                                                     || currentDriver.currentOrder -> getStatus () == OrderStatus::paid) {
        switch (userInput) {
            case 1 : currentDriver.readMesseges (); goToDriverScreen (); break;
            case 2 :
                if (!currentDriver.currentOrder.isInitialised () || currentDriver.currentOrder -> getStatus () == OrderStatus::cancelled
                                                                 || currentDriver.currentOrder -> getStatus () == OrderStatus::concluded) {
                    currentDriver.changePosition ();
                    goToDriverScreen ();
                }
                else if (currentDriver.currentOrder -> getStatus () == OrderStatus::recieved) {
                    currentDriver.finishOrder ();
                    Point originPoint = (currentDriver.currentOrder -> getOrigin ()).getPoint ();
                    Point destinationPoint = (currentDriver.currentOrder -> getDestination ()).getPoint ();
                    double distanceFromOriginToDestination = originPoint.getDistance (destinationPoint);
                    double cost = distanceFromOriginToDestination * UberTaxiDriver::tariff;
                    if (cost < UberTaxiDriver::minimalPrice) {
                        cost = UberTaxiDriver::minimalPrice;
                    }
                    currentDriver.setPosition (destinationPoint);
                    currentDriver.currentOrder -> setCost (cost);
                    currentDriver.currentOrder -> setStatus (OrderStatus::completed);
                    goToDriverScreen ();
                }
                else if (currentDriver.currentOrder -> getStatus () == OrderStatus::paid) {
                    currentDriver.acceptPayment ();
                    uberSystem.totalIncomeOfCompany += currentDriver.currentOrder -> getMoneyPaid () * (1 - UberTaxiDriver::percentOfIncome);
                    currentDriver.currentOrder -> setStatus (OrderStatus::concluded);
                    currentDriver.isFreeAtTheMoment = true;
                    goToDriverScreen ();
                }
                break;
            case 3 : cout << "You have " << std::fixed << std::setprecision (2) << currentDriver.getIncome () << " " << CURRENCY << ".\n"; goToDriverScreen (); break;
            case 4 : cout << "You have " << std::fixed << std::setprecision (2) << currentDriver.getRating () << " rating.\n"; goToDriverScreen (); break;
            case 5 : goToMainScreen (); break;
            case 6 : goToDriverScreen (); break;
        }                              
    }
    else if (currentDriver.currentOrder -> getStatus () == OrderStatus::registered) {
        switch (userInput) {
            case 1 : currentDriver.readMesseges (); goToDriverScreen (); break;
            case 2 : 
                {
                    unsigned int minutesToArrive = currentDriver.acceptOrder ();
                    currentDriver.currentOrder -> setMinutesToArrive (minutesToArrive);
                    currentDriver.currentOrder -> setDriverData (DriverData (currentDriver.getUsername (), currentDriver.getFullName (), currentDriver.getCarNumber (),
                                                                             currentDriver.getTelephoneNumber (), currentDriver.getRating ()));
                    currentDriver.currentOrder -> setStatus (OrderStatus::recieved);
                    currentDriver.isFreeAtTheMoment = false;
                    goToDriverScreen (); break;
                }
            case 3 : 
                {
                    currentDriver.declineOrder ();
                    currentDriver.currentOrder -> numberOfDriversToDecline += 1;
                    currentDriver.isFreeAtTheMoment = false;
                    int closestDriver = uberSystem.findClosetDriver (*currentDriver.currentOrder);
                    currentDriver.isFreeAtTheMoment = true;
                    if (closestDriver != -1) {
                        uberSystem.arrayOfDrivers[closestDriver].currentOrder = std::move(currentDriver.currentOrder);
                    }
                    goToDriverScreen (); break;
                }
            case 4 : cout << "You have " << std::fixed << std::setprecision (2) << currentDriver.getIncome () << " " << CURRENCY << ".\n"; goToDriverScreen (); break;
            case 5 : cout << "You have " << std::fixed << std::setprecision (2) << currentDriver.getRating () << " rating.\n"; goToDriverScreen (); break;
            case 6 :  goToMainScreen (); break;
        }
    }
    else if (currentDriver.currentOrder -> getStatus () == OrderStatus::completed) {
        switch (userInput) {
            case 1 : currentDriver.readMesseges (); goToDriverScreen (); break;
            case 2 : cout << "You have " << std::fixed << std::setprecision (2) << currentDriver.getIncome () << " " << CURRENCY << ".\n"; goToDriverScreen (); break;
            case 3 : cout << "You have " << std::fixed << std::setprecision (2) << currentDriver.getRating () << " rating.\n"; goToDriverScreen (); break;
            case 4 : goToMainScreen (); break;
            case 5 : goToDriverScreen (); break;
            case 6 : goToDriverScreen (); break;
        }
    }
}