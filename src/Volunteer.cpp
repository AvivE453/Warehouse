#include "../include/Parse.h"
#include "../include/WareHouse.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
using namespace std;

Volunteer::Volunteer(int id, const string &name) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name)
{
}

int Volunteer::getId() const
{
    return id;
}
const string &Volunteer::getName() const
{
    return name;
}
int Volunteer::getActiveOrderId() const
{
    return activeOrderId;
}
int Volunteer::getCompletedOrderId() const
{
    return completedOrderId;
}
bool Volunteer::isBusy() const
{
    return activeOrderId != NO_ORDER;
}
void Volunteer::setActiveOrderId(int orderId)
{
    activeOrderId = orderId;
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown)
{
}

CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}
void CollectorVolunteer::step()
{
    decreaseCoolDown();
}
int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}
int CollectorVolunteer::getTimeLeft() const
{
    return timeLeft;
}
bool CollectorVolunteer::decreaseCoolDown()
{
    timeLeft--;
    return timeLeft == 0;
} // Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}
bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return !isBusy() && hasOrdersLeft() && order.getStatus() == OrderStatus::PENDING;
}
void CollectorVolunteer::acceptOrder(const Order &order)
{

    activeOrderId = order.getId();
    timeLeft = coolDown;
}
bool CollectorVolunteer::hasFininshed()
{
    return timeLeft <= 0;
}

string CollectorVolunteer::toString() const
{
    return "Collector Volunteer: " + getName() + "id: " + to_string(getId());
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders)
{
}
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}
void LimitedCollectorVolunteer::step()
{
    CollectorVolunteer::step();
}
bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
    return CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft();
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}
string LimitedCollectorVolunteer::toString() const
{
    return "Limited Collector Volunteer: " + getName() + "id: " + to_string(getId());
}

DriverVolunteer ::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0)
{
}
DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}

int DriverVolunteer ::getDistanceLeft() const
{
    return distanceLeft;
}
int DriverVolunteer ::getMaxDistance() const
{
    return maxDistance;
}
int DriverVolunteer ::getDistancePerStep() const
{
    return distancePerStep;
}
bool DriverVolunteer ::decreaseDistanceLeft()
{ // Decrease distanceLeft by distancePerStep,return true if  distanceLeft<=0,false otherwise
    distanceLeft -= distancePerStep;
    if (distanceLeft <= 0)
    {
        distanceLeft = 0;
        return true;
    }
    return false;
}
bool DriverVolunteer ::hasOrdersLeft() const
{
    return true;
}
bool DriverVolunteer ::canTakeOrder(const Order &order) const // Signal if the volunteer is not busy and the order is within the maxDistance
{
    return !isBusy() && hasOrdersLeft() && order.getDistance() <= maxDistance && order.getStatus() == OrderStatus::COLLECTING;
}
void DriverVolunteer ::acceptOrder(const Order &order) // Assign distanceLeft to order's distance
{
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}
void DriverVolunteer ::step() // Decrease distanceLeft by distancePerStep
{
    decreaseDistanceLeft();
}
bool DriverVolunteer::hasFininshed()
{
    return distanceLeft <= 0;
}
string DriverVolunteer ::toString() const
{
    return "Driver Volunteer: " + getName() + "id: " + to_string(getId());
}
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}
LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(*this);
}

void LimitedDriverVolunteer::step()
{
    DriverVolunteer::step();
}
int LimitedDriverVolunteer::getMaxOrders() const
{
    return maxOrders;
}
int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}
bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const // Signal if the volunteer is not busy, the order is within the maxDistance.
{
    return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft();
}
void LimitedDriverVolunteer::acceptOrder(const Order &order) // Assign distanceLeft to order's distance and decrease ordersLeft
{
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}
string LimitedDriverVolunteer::toString() const
{
    return "Limited Driver Volunteer: " + getName() + "id: " + to_string(getId());
}