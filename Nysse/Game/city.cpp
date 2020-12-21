﻿#include "city.hh"
#include <QDebug>
#include <vector>


namespace StudentSide
{

City::City()
{

}


City::~City()
{

}


void City::setBackground(QImage &basicbackground, QImage &bigbackground)
{
    small_ = basicbackground;
    big_ = bigbackground;
}


void City::setClock(QTime clock)
{
    time_ = clock;

}


void City::addStop(std::shared_ptr<Interface::IStop> stop)
{
    Q_ASSERT(stop != nullptr);
    Interface::Location location = stop->getLocation();
    if(location.giveX() > RENDER_DISTANCE && location.giveX() < RENDER_DISTANCE && location.giveY() > RENDER_DISTANCE && location.giveY() < RENDER_DISTANCE) {
        throw Interface::InitError("Stops position is not valid.");
    }
    if(std::find(stops_.begin(), stops_.end(), stop) == stops_.end()){
        stops_.push_back(stop);
        ui_->addStop(stop);
    }
}


void City::startGame()
{
    gamestarted = true;
}


void City::addActor(std::shared_ptr<Interface::IActor> newactor)
{
    if(std::find(actors_.begin(), actors_.end(), newactor) == actors_.end()){
        Interface::Location location = newactor->giveLocation();
        if(std::shared_ptr<Interface::IPassenger> passenger = std::dynamic_pointer_cast<Interface::IPassenger>(newactor)) {
            actors_.push_back(newactor);
            if (location.giveX() > PASSENGER_RENDER_MIN && location.giveX() < PASSENGER_RENDER_MAX && location.giveY() > PASSENGER_RENDER_MIN && location.giveY() < PASSENGER_RENDER_MAX) {
                new_passengers.push_back(newactor);
            }
        }
        else {
            //adding new busses traight away to ui
            actors_.push_back(newactor);
            ui_->addActor(newactor);
            stats_->newNysse();
        }
    }
    else {
        throw Interface::GameError("Actor is already in the city.");
    }
}


void City::removeActor(std::shared_ptr<Interface::IActor> actor)
{
    if(std::find(actors_.begin(), actors_.end(), actor) != actors_.end()){
        actor->remove();
        actors_.erase(std::remove(actors_.begin(), actors_.end(), actor), actors_.end());
        ui_->removeActor(actor, false);
        stats_->nysseLeft();
    }
    else {
        throw Interface::GameError("Actor not found in the city");
    }
}


void City::actorRemoved(std::shared_ptr<Interface::IActor> actor)
{
    Q_ASSERT(actor != nullptr);
    qDebug() << actor->isRemoved();
}


bool City::findActor(std::shared_ptr<Interface::IActor> actor) const
{
    if(std::find(actors_.begin(), actors_.end(), actor) != actors_.end()){
        return true;
    }
    return false;
}


std::vector<std::shared_ptr<Interface::IActor> > City::getNearbyActors(Interface::Location loc) const
{
    std::vector<std::shared_ptr<Interface::IActor>> actorsToBedeleted;
    for(unsigned long int i = 0; i < actors_.size(); i++) {
        Interface::Location location = actors_.at(i)->giveLocation();
        Interface::Location customLocation = loc;
        customLocation.setXY(X_COMP + location.giveX(), Y_COMP - location.giveY());

        if(customLocation.isClose(loc, DISTANCE_TO_INTERACT) == true){
            actorsToBedeleted.push_back(actors_[i]);
        }
    }
    return actorsToBedeleted;
}


void City::actorMoved(std::shared_ptr<Interface::IActor> actor)
{
    Q_ASSERT(actor != nullptr);
    moved_actor.push_back(actor);
}


bool City::isGameOver() const
{
    return gameOver;
}


QImage City::getImage(std::string image_size)
{
    if(image_size == "small") {
        return small_;
    }
    else {
        return big_;
    }
}


void City::addUi(std::shared_ptr<StudentSide::Mainwindow> ui)
{
    Q_ASSERT(ui != nullptr);
    ui_ = ui;
}


void City::makePlayer()
{
    Interface::Location* location =  new Interface::Location;
    player_ = std::make_shared<StudentSide::Actor>(StudentSide::Actor());
    player_->addLocation(location);
    ui_->addPlayer(player_);
}


void City::removeActorsNearby(std::shared_ptr<Interface::IActor> actor)
{
    Q_ASSERT(actor != nullptr);
    if(std::find(actors_.begin(), actors_.end(), actor) != actors_.end()) {
        if(std::shared_ptr<Interface::IPassenger> passenger = std::dynamic_pointer_cast<Interface::IPassenger>(actor)) {
            if(passenger->isInVehicle() == true) {
                actor->remove();
                actors_.erase(std::remove(actors_.begin(), actors_.end(), actor), actors_.end());
                ui_->removeActor(actor, true);
            }
        }
        else if(actor == nuke_) {
             actors_.erase(std::remove(actors_.begin(), actors_.end(), actor), actors_.end());
             ui_->removeActor(nuke_, false);
        }
    }
}


std::vector<std::shared_ptr<Interface::IActor> > City::giveMovedActors()
{
    std::vector<std::shared_ptr<Interface::IActor>> actors = moved_actor;
    moved_actor.clear();
    return actors;
}


std::vector<std::shared_ptr<Interface::IActor> > City::giveNewPassengers()
{
    std::vector<std::shared_ptr<Interface::IActor>> actors = new_passengers;
    new_passengers.clear();
    return actors;
}


void City::takeStats(std::shared_ptr<Statistics> stats)
{
    stats_ = stats;
}


void City::addNuke()
{
    Interface::Location* location =  new Interface::Location;
    nuke_ = std::make_shared<StudentSide::Actor>(StudentSide::Actor());
    location->setXY(600, 200);
    nuke_->addLocation(location);
    actors_.push_back(nuke_);
    ui_->addNuke(nuke_);

}


void City::nukeCity()
{
    for(unsigned long int i = 0; i < actors_.size(); i++) {
        if(actors_.at(i)->isRemoved() == false) {
            actors_.at(i)->remove();
            ui_->removeActor(actors_.at(i), true);
            if(std::shared_ptr<Interface::IVehicle> NYSSE = std::dynamic_pointer_cast<Interface::IVehicle>(actors_.at(i))) {
                stats_->nysseRemoved();
            }
            actors_.erase(std::remove(actors_.begin(), actors_.end(), actors_.at(i)), actors_.end());
        }
    }
}


void City::GameOver()
{
    gameOver = true;
}

} // namespace
