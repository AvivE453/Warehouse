#include "../include/Action.h"
#include <iostream>
#include "../include/WareHouse.h"
#include "../include/Order.h"
#include "../include/Volunteer.h"
using namespace std;

extern WareHouse *backup;

BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED)
{
}

ActionStatus BaseAction::getStatus() const
{
  return status;
}

void BaseAction::complete()
{
  status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
  status = (ActionStatus::ERROR);
  cout << "Error: " << errorMsg << endl;
  this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const
{
  return errorMsg;
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : customerName(customerName), customerType(convertCustomerType(customerType)), distance(distance), maxOrders(maxOrders)
{
}

void AddCustomer::act(WareHouse &wareHouse)
{
  Customer *customer;
  if (customerType == CustomerType::Soldier)
  {
    customer = new SoldierCustomer(wareHouse.assignCustomerId(), customerName, distance, maxOrders);
  }
  else
  {
    customer = new CivilianCustomer(wareHouse.assignCustomerId(), customerName, distance, maxOrders);
  }
  wareHouse.addCustomer(customer);
  complete();
}

AddCustomer *AddCustomer::clone() const
{
  return new AddCustomer(*this);
}
string AddCustomer::toString() const
{
  string type = "soldier";
  if (customerType == CustomerType::Civilian)
    type = "civilian";
  string statusType = "ERROR";
  if (getStatus() == ActionStatus::COMPLETED)
    statusType = "COMPLETED";
  return "customer " + customerName + " " + type + " " + to_string(distance) + " " + to_string(maxOrders) + " " + statusType;
}

CustomerType AddCustomer::convertCustomerType(const string &customerType) const
{
  if (customerType == "soldier")
    return CustomerType::Soldier;
  return CustomerType::Civilian;
}
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId)
{
}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
  if (customerId < wareHouse.getCustomerCounter())
  {
    Customer *customer = &wareHouse.getCustomer(customerId);
    cout << "CustomerID: " << to_string(customer->getId()) << endl;
    for (int id : customer->getOrdersIds())
    {
      OrderStatus status(wareHouse.getOrder(id).getStatus());
      string status2 = "";
      cout << "OrderID: " << to_string(id) << endl;
      switch (status)
      {
      case OrderStatus::COMPLETED:
        status2 = "COMPLETED";
        break;
      case OrderStatus::PENDING:
        status2 = "PENDING";
        break;
      case OrderStatus::COLLECTING:
        status2 = "COLLECTING";
        break;
      case OrderStatus::DELIVERING:
        status2 = "DELIVERING";
        break;
      }
      cout << "OrderStatus: " << status2 << endl;
    }
    cout << "numOrdersLeft: " << to_string(customer->getMaxOrders() - customer->getOrdersIds().size()) << endl;
    complete();
  }
  else
  {
    error("customer doesn't exist");
  }
}
PrintCustomerStatus *PrintCustomerStatus::clone() const
{
  return new PrintCustomerStatus(*this);
}
string PrintCustomerStatus::toString() const
{
  string statusType = "ERROR";
  if (getStatus() == ActionStatus::COMPLETED)
    statusType = "COMPLETED";
  return "customerStatus " + to_string(customerId) + " " + statusType;
}
AddOrder::AddOrder(int id) : customerId(id)
{
}

void AddOrder::act(WareHouse &wareHouse)
{
  Customer *customer = &wareHouse.getCustomer(customerId);
  if (customerId < wareHouse.getCustomerCounter() && customer->canMakeOrder())
  {
    Order *order = new Order(wareHouse.assignOrderId(), customerId,
                             customer->getCustomerDistance());
    wareHouse.addOrder(order);
    complete();
  }
  else
    error("Cannot place this order");
}
string AddOrder::toString() const
{
  string statusType = "ERROR";
  if (getStatus() == ActionStatus::COMPLETED)
    statusType = "COMPLETED";
  return "order " + to_string(customerId) + " " + statusType;
}
AddOrder *AddOrder::clone() const
{
  return new AddOrder(*this);
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
  for (int step = 1; step <= numOfSteps; step++)
  {
    const vector<Volunteer *> &voulnteers = wareHouse.getVolunteersList();
    const vector<Order *> &orders = wareHouse.getPendingOrders();
    for (size_t j = 0; j < orders.size(); j++)
    {
      for (size_t i = 0; i < voulnteers.size(); i++)
      {
        if (voulnteers[i]->canTakeOrder(*orders[j]))
        {
          voulnteers[i]->acceptOrder(*orders[j]);
          if (orders[j]->getStatus() == OrderStatus::PENDING)
          {
            orders[j]->setStatus(OrderStatus::COLLECTING); // the status is changed only when new voulnteer accept the order
            orders[j]->setCollectorId(voulnteers[i]->getId());
          }
          else
          {
            orders[j]->setStatus(OrderStatus::DELIVERING);
            orders[j]->setDriverId(voulnteers[i]->getId());
          }
          wareHouse.addInProcessOrderToList(orders[j]);
          wareHouse.removePendingOrderFromList(orders[j]);
          j--;
          break;
        }
      }
    }

    for (size_t i = 0; i < voulnteers.size(); i++)
    {
      if (voulnteers[i]->isBusy())
      {
        voulnteers[i]->step();
        if (voulnteers[i]->hasFininshed())
        {
          Order *order = &wareHouse.getOrder(voulnteers[i]->getActiveOrderId());
          if (order->getStatus() == OrderStatus::DELIVERING)
          {
            wareHouse.addCompletedOrderToList(order);
            order->setStatus(OrderStatus::COMPLETED);
          }
          else
          {
            wareHouse.addPendingOrderToList(order);
          }
          voulnteers[i]->setActiveOrderId(NO_ORDER);
          wareHouse.removeInProcessOrderFromList(order);
          if (typeid(*voulnteers[i]) == typeid(LimitedCollectorVolunteer) || typeid(*voulnteers[i]) == typeid(LimitedDriverVolunteer))
          {
            if (!voulnteers[i]->hasOrdersLeft())
            {
              wareHouse.removeVolunteerFromList(voulnteers[i]);
            }
          }
        }
      }
    }
  }
  complete();
}
std::string SimulateStep::toString() const
{
  string statusType = "ERROR";
  if (getStatus() == ActionStatus::COMPLETED)
    statusType = "COMPLETED";
  return "simulateStep " + to_string(numOfSteps) + " " + statusType;
}
SimulateStep *SimulateStep::clone() const
{
  return new SimulateStep(*this);
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}
void PrintOrderStatus::act(WareHouse &wareHouse)
{
  if (orderId >= 0 && orderId < wareHouse.getOrderCounter())
  {
    Order order(wareHouse.getOrder(orderId));
    cout << "OrderId: " << orderId << endl;
    string status;
    switch (order.getStatus())
    {
    case OrderStatus::PENDING:
      status = "Pending";
      break;
    case OrderStatus::DELIVERING:
      status = "Delivering";
      break;
    case OrderStatus::COLLECTING:
      status = "Collecting";
      break;
    case OrderStatus::COMPLETED:
      status = "Completed";
      break;
    }
    cout << "OrderStatus: " << status << endl;
    cout << "CustomerID: " << order.getCustomerId() << endl;
    string collectorId = order.getStatus() != OrderStatus::PENDING ? to_string(order.getCollectorId()) : "None";
    cout << "CollectorID: " << collectorId << endl;
    string driverId = order.getStatus() == OrderStatus::DELIVERING || order.getStatus() == OrderStatus::COMPLETED ? to_string(order.getDriverId()) : "None";
    cout << "DriverID: " << driverId << endl;
    complete();
  }
  else
  {
    error("Order doesn't exist");
  }
}
PrintOrderStatus *PrintOrderStatus::clone() const
{
  return new PrintOrderStatus(*this);
}
string PrintOrderStatus::toString() const
{
  string statusType = "ERROR";
  if (getStatus() == ActionStatus::COMPLETED)
    statusType = "COMPLETED";
  return "orderStatus " + to_string(orderId) + " " + statusType;
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id)
{
}
void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
  if (volunteerId >= 0 && volunteerId < wareHouse.getVolunteerCounter())
  {
    Volunteer *volunteer(&wareHouse.getVolunteer(volunteerId));
    if (volunteer->getId() != -1)
    {
      cout << "VolunteerId: " << volunteerId << endl;
      string isBusy = "False";
      string orderID = "None";
      if (volunteer->isBusy())
      {
        isBusy = "True";
        orderID = to_string(volunteer->getActiveOrderId());
      }
      cout << "isBusy: " << isBusy << endl;

      cout << "OrderID: " << orderID << endl;
      string time = "None";
      string ordersLimit = "No Limit";
      if (typeid(*volunteer) == typeid(CollectorVolunteer))
      {
        time = to_string(dynamic_cast<CollectorVolunteer *>(volunteer)->getTimeLeft());
      }

      else if (typeid(*volunteer) == typeid(LimitedCollectorVolunteer))
      {
        time = to_string(dynamic_cast<LimitedCollectorVolunteer *>(volunteer)->getTimeLeft());
        ordersLimit = to_string(dynamic_cast<LimitedCollectorVolunteer *>(volunteer)->getNumOrdersLeft());
      }
      else if (typeid(*volunteer) == typeid(DriverVolunteer))
      {
        time = to_string(dynamic_cast<DriverVolunteer *>(volunteer)->getDistanceLeft());
      }
      else if (typeid(*volunteer) == typeid(LimitedDriverVolunteer))
      {
        time = to_string(dynamic_cast<LimitedDriverVolunteer *>(volunteer)->getDistanceLeft());
        ordersLimit = to_string(dynamic_cast<LimitedDriverVolunteer *>(volunteer)->getNumOrdersLeft());
      }
      cout << "TimeLeft: " << time << endl;
      cout << "OrdersLeft: " << ordersLimit << endl;
      complete();
    }
    else
    {
      error("Volunteer doesn't exist");
    }
  }
  else
  {
    error("Volunteer doesn't exist");
  }
}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
  return new PrintVolunteerStatus(*this);
}
string PrintVolunteerStatus::toString() const
{
  string statusType = "ERROR";
  if (getStatus() == ActionStatus::COMPLETED)
    statusType = "COMPLETED";
  return "volunteerStatus " + to_string(volunteerId) + " " + statusType;
}
PrintActionsLog::PrintActionsLog()
{
}
void PrintActionsLog::act(WareHouse &wareHouse)
{
  vector<BaseAction *> actionsLog = wareHouse.getActionsLog();
  for (size_t i = 0; i < actionsLog.size(); i++)
  {
    cout << actionsLog[i]->toString() << endl;
  }
}
PrintActionsLog *PrintActionsLog::clone() const
{
  return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const
{
  return "log COMPLETED ";
}
Close::Close()
{
}
void Close::act(WareHouse &wareHouse)
{
  vector<Order *> orders = wareHouse.getPendingOrders();
  for (size_t i = 0; i < orders.size(); i++)
  {
    cout << orders[i]->toString() << endl;
  }
  orders = wareHouse.getCompletedOrders();
  for (size_t i = 0; i < orders.size(); i++)
  {
    cout << orders[i]->toString() << endl;
  }
  orders = wareHouse.getInProcessOrders();
  for (size_t i = 0; i < orders.size(); i++)
  {
    cout << orders[i]->toString() << endl;
  }
}
Close *Close::clone() const
{
  return new Close(*this);
}
string Close::toString() const
{
  return "Close Completed";
}
BackupWareHouse::BackupWareHouse()
{
}
void BackupWareHouse::act(WareHouse &wareHouse)
{
  if (backup != nullptr)
    delete backup;
  backup = wareHouse.clone();
}
BackupWareHouse *BackupWareHouse::clone() const
{
  return new BackupWareHouse(*this);
}
string BackupWareHouse::toString() const
{
  return "Backup COMPLETED ";
}
RestoreWareHouse::RestoreWareHouse()
{
}
void RestoreWareHouse::act(WareHouse &wareHouse)
{
  if (backup != nullptr)
  {
    wareHouse = *backup;
    complete();
  }
  else
  {
    error("No backup available");
  }
}
RestoreWareHouse *RestoreWareHouse::clone() const
{
  return new RestoreWareHouse(*this);
}
string RestoreWareHouse::toString() const
{
  string statusType = "ERROR";
  if (getStatus() == ActionStatus::COMPLETED)
    statusType = "COMPLETED";
  return "Restore " + statusType;
}