#include "tasks/Draw.h"

#include "components/TransformComponent.h"
#include "components/PlayerComponent.h"
#include "components/NPCComponent.h"
#include "components/BulletComponent.h"

#include "PresentationManager.h"
#include "EntitySystem.h"
#include "GameInfo.h"

void DrawTask::Tick()
{
    PresentationManager::BeginLayer(BackgroundLayer);
    DrawRectangleGradientEx(PresentationManager::GetCurrentLayerRect(), BLACK, BLACK, Color(0, 0, 40, 255), Color(40,40,40,255));
    PresentationManager::EndLayer();

    PresentationManager::BeginLayer(PlayerLayer);
    EntitySystem::DoForEachComponent<PlayerComponent>([&](auto& player)
        {
            auto transform = player.GetEntityComponent<TransformComponent>();
            if (transform)
            {
                player.Data.Sprite.Draw(transform->Data.Position, WHITE);
            }
        });

    EntitySystem::DoForEachComponent<BulletComponent>([&](auto& bullet)
        {
            auto transform = bullet.GetEntityComponent<TransformComponent>();
            if (transform)
            {
                bullet.Data.Sprite.Draw(transform->Data.Position, bullet.Data.Tint);
            }
        });
    PresentationManager::EndLayer();

    PresentationManager::BeginLayer(NPCLayer);
    double now = GetTime();
    EntitySystem::DoForEachComponent<NPCComponent>([&](NPCComponent& npc)
        {
            auto transform = npc.GetEntityComponent<TransformComponent>();
            if (transform)
            {
                Vector2 interpPos = transform->Data.Position - Vector2(npc.Data.Size, npc.Data.Size);
                if (UseInterpolateNPCs)
                    interpPos += transform->Data.Velocity * float(now - npc.LastUpdateTime);

                npc.Data.Sprite.Scale = npc.Data.Size / 2.0f;
                npc.Data.Sprite.Draw(interpPos, npc.Data.Tint);
            }
        });
    PresentationManager::EndLayer();
}
