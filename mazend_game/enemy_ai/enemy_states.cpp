#include "enemy_states.h"
#include <LevelSystem.h>
#include "../components/cmp_enemy_movement.h"
#include "../components/cmp_player_movement.h"

void CasualState::Execute(Entity* owner, double dt) noexcept {
    if (owner->GetComponents<EnemyMovementComponent>()[0]->getSector() == _player->GetComponents<PlayerMovementComponent>()[0]->getSector()) {
        auto sm = owner->GetComponents<StateMachineComponent>();
        sm[0]->changeState("hunting");
    }
}

void HuntingState::Execute(Entity* owner, double dt) noexcept {
    owner->GetComponents<EnemyMovementComponent>()[0]->setHunting(true);
}
