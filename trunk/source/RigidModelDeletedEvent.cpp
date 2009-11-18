#include "RigidModelEntity.h"
#include "RigidModelDeletedEvent.h"

RigidModelDeletedEvent::RigidModelDeletedEvent(const std::string &name, void *ptr)
: GameEvent(name)
{
	mPtrToDelete = ptr;
}

void* RigidModelDeletedEvent::getPointer() const {
	return mPtrToDelete;
}