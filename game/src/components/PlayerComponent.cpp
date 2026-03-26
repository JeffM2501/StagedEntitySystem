#include "components/PlayerComponent.h"
#include "components/TransformComponent.h"
#include "GameInfo.h"

#include "TimeUtils.h"

void PlayerComponent::OnAwake()
{
}

bool PlayerComponent::OnDataRead(BufferReader& buffer)
{
    Data.Read(buffer);

    TraceLog(LOG_INFO, "Loaded PlayerComponent for entity %zu", EntityID);
    return true;
}

void PlayerComponent::Update()
{
    auto transform = GetEntityComponent<TransformComponent>();
    {
        transform->Data.Position += Input * Data.PlayerSpeed * GetDeltaTime();

        LastShotTime += GetDeltaTime();
        if (ShootThisFrame)
        {
            if (LastShotTime > Data.ReloadTime)
            {
                LastShotTime = 0;

                Vector2 pos = transform->Data.Position;

                PrefabReader.ReadEntitiesFromResource(Data.BulletPrefab, [pos, this](std::span<size_t> entities)
                    {
                        auto bulletTransform = EntitySystem::GetEntityComponent<TransformComponent>(entities[0]);
                        if (bulletTransform)
                        {
                            bulletTransform->Data.Position = pos;
                            float speed = Data.PlayerSpeed * Data.ShotSpeedMultiplyer + float(GetRandomValue(0, int(Data.PlayerSpeed * Data.ShotSpeedVariance)));

                            bulletTransform->Data.Velocity = Vector2(speed, float(GetRandomValue(int(-Data.ShotSpread), int(Data.ShotSpread)))) + (Input * Data.PlayerSpeed);
                        }
                    });
            }
        }
    }

    ShootThisFrame = false;
}