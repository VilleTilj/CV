#ifndef VEHICLE_HH
#define VEHICLE_HH
#include "interfaces/ivehicle.hh"
#include "interfaces/ipassenger.hh"
#include "QDebug"
#include "errors/gameerror.hh"

namespace StudentSide {

/*!
 * \file vehicle.hh
 * \brief The Vehicle class
 */

class Vehicle : public Interface::IVehicle
{
public:

    /*!
     * \brief Vehicle contructor
     */
    Vehicle();

    /*!
     * \brief Destructor
     */
    ~Vehicle();

    /*!
     * \brief getName returns the name of the vehicle
     * \return the name of vehicle
     */
    std::string getName() const override;

    /*!
     * \brief getPassengers returns vector containing passengers
     * \return vector containing passengers
     */
    std::vector<std::shared_ptr<Interface::IPassenger> > getPassengers() const override;

    /*!
     * \brief addPassenger adds passenger to vehicle
     * \param passenger
     */
    void addPassenger(std::shared_ptr<Interface::IPassenger> passenger) override;

    /*!
     * \brief removePassenger removes passenger form vehicle
     * \param passenger
     */
    void removePassenger(std::shared_ptr<Interface::IPassenger> passenger) override;

private:

    std::vector<std::shared_ptr<Interface::IPassenger> > passengers; //!< vector containing all the passenger in vehicle
    std::string name = ""; //!< name of the vehicle
};



} //namespace

#endif // VEHICLE_HH
