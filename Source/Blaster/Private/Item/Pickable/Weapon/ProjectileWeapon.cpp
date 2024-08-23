// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickable/Weapon/ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Actor/Projectile/Projectile.h"
#include "Kismet/KismetMathLibrary.h"

//#include "GameMode/BlasterGameMode.h"
#include "GameState/BlasterGameState.h"
#include "Components/ObjectPoolComponent.h"
//#include "GameFramework/ProjectileMovementComponent.h"
#include "Actor/Projectile/ProjectileBullet.h"

AProjectileWeapon::AProjectileWeapon()
{
}

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	//UE_LOG(LogTemp, Display, TEXT("%s : Fire Grenade Launcher"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());


	//if (!HasAuthority()) return;

	// �Ʒ� �Ѿ��� ����� �߻��ϴ� ���� �ſ� �ΰ��� �ൿ���̹Ƿ� ������ ���������� ������ �� �ְ� �Ѵ�.
	// �ذ�å�� �Ѿ��� bReplicates = true �� ����� �������� �߻��ϸ� Ŭ�󿡵� �Ȱ��� �Ѿ��� �����ǵ��� �ϸ� �ȴ�!
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(TEXT("MuzzleFlash"));
	UWorld* World = GetWorld();

	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;

		AProjectile* SpawnedProjectile = nullptr;
		if (bUseServerSideRewind)
		{
			if (InstigatorPawn->HasAuthority()) // server
			{
				if (InstigatorPawn->IsLocallyControlled()) // server, host - use replicated projectile
				{
					//UE_LOG(LogTemp, Display, TEXT("server, host"));

					//SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					//SpawnedProjectile->bUseServerSideRewind = false;
					//SpawnedProjectile->Damage = Damage;
					//SpawnedProjectile->HeadShotDamage = HeadShotDamage;

					//APooledObject* PO = Cast<UObjectPoolSubsystem>(GetGameInstance())->GetSpawnedObject(FTransform(TargetRotation, SocketTransform.GetLocation()), ProjectileClass);

					//APooledObject* PO = GetWorld()->GetAuthGameMode<ABlasterGameMode>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(FTransform(TargetRotation, SocketTransform.GetLocation()), ProjectileClass);
					//if (PO)
					//{
					//	Cast<AProjectileBullet>(PO)->SetProjectileMovementVelocity(ToTarget);
					//	PO->SetOwner(GetOwner());
					//	PO->SetInstigator(InstigatorPawn);
					//}


					APooledObject* PO = GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(FTransform(TargetRotation, SocketTransform.GetLocation()), ProjectileClass);
					if (PO)
					{
						Cast<AProjectileBullet>(PO)->SetProjectileMovementVelocity(ToTarget);
						PO->SetOwner(GetOwner());
						PO->SetInstigator(InstigatorPawn);
					}

				}
				else // server, not locally controlled  - spawn non-replicated projectile, SSR
				{
					//UE_LOG(LogTemp, Display, TEXT("server, not locally controlled"));
					//SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					//SpawnedProjectile->bUseServerSideRewind = true;

					APooledObject* PO = GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(FTransform(TargetRotation, SocketTransform.GetLocation()), ServerSideRewindProjectileClass);
					if (PO)
					{
						Cast<AProjectileBullet>(PO)->SetProjectileMovementVelocity(ToTarget);
						PO->SetOwner(GetOwner());
						PO->SetInstigator(InstigatorPawn);
					}
				}
			}
			else // client, using SSR
			{
				if (InstigatorPawn->IsLocallyControlled()) // client, locally controlled - spawn non-replicated projectile, use SSR
				{
					//UE_LOG(LogTemp, Display, TEXT("client, locally controlled"));
					//SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					//SpawnedProjectile->bUseServerSideRewind = true;
					//SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
					//SpawnedProjectile->InitialVelocity = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;


					APooledObject* PO = GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(FTransform(TargetRotation, SocketTransform.GetLocation()), ServerSideRewindProjectileClass);
					if (PO)
					{
						Cast<AProjectileBullet>(PO)->SetProjectileMovementVelocity(ToTarget);
						PO->SetOwner(GetOwner());
						PO->SetInstigator(InstigatorPawn);
					}
				}
				else // client, not locally controlled - spawn non-replicated projectile, no SSR
				{
					//UE_LOG(LogTemp, Display, TEXT("client, not locally controlled"));

					//SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					//SpawnedProjectile->bUseServerSideRewind = false;

					APooledObject* PO = GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(FTransform(TargetRotation, SocketTransform.GetLocation()), ServerSideRewindProjectileClass);
					if (PO)
					{
						Cast<AProjectileBullet>(PO)->SetProjectileMovementVelocity(ToTarget);
						PO->SetOwner(GetOwner());
						PO->SetInstigator(InstigatorPawn);
					}
				}
			}
		}
		else // weapon not using SSR
		{
			// client, locally controlled - spawn non-replicated projectile, use SSR
			//if (InstigatorPawn->HasAuthority())

			//if (HasAuthority())
			{
				//UE_LOG(LogTemp, Display, TEXT("Not SSR, Authority"));
				//SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);

				//SpawnedProjectile->bUseServerSideRewind = false;
				//SpawnedProjectile->Damage = Damage;


				APooledObject* PO = GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(FTransform(TargetRotation, SocketTransform.GetLocation()), ProjectileClass);
				if (PO)
				{
					Cast<AProjectile>(PO)->SetProjectileMovementVelocity(ToTarget);
					PO->SetOwner(GetOwner());
					PO->SetInstigator(InstigatorPawn);
				}
			}
		}

	}
}
