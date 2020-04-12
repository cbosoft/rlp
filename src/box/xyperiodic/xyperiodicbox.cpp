#include "xyperiodicbox.hpp"

XYPeriodicBox::XYPeriodicBox(double L, int verbosity, double friction_thresh)
  : GenericBox(L, verbosity, friction_thresh) 
{
  this->logger = Logger(nullptr, verbosity);
};


std::list<ParticleArrangement *> XYPeriodicBox::get_arrangements() const
{
  return this->arrangements;
}


int XYPeriodicBox::get_number_arrangements() const
{
  for (auto arr : this->arrangements) {
    std::cerr << arr->repr() << std::endl;
  }
  return this->arrangements.size();
}


XYPeriodicBox::~XYPeriodicBox()
{
  this->clear_arrangements();
}


void XYPeriodicBox::clear_arrangements()
{
  for (ParticleArrangement *a : this->arrangements) {
    delete a;
  }
}
