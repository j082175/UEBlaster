// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickable/Weapon/ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Actor/Projectile/Projectile.h"
#include "Kismet/KismetMathLibrary.h"

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
					SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = false;
					SpawnedProjectile->Damage = Damage;
					SpawnedProjectile->HeadShotDamage = HeadShotDamage;
				}
				else // server, not locally controlled  - spawn non-replicated projectile, SSR
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = true;

				}
			}
			else // client, using SSR
			{
				if (InstigatorPawn->IsLocallyControlled()) // client, locally controlled - spawn non-replicated projectile, use SSR
				{
					//UE_LOG(LogTemp, Display, TEXT("Autonomous"));
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = true;
					SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
					SpawnedProjectile->InitialVelocity = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
				}
				else // client, not locally controlled - spawn non-replicated projectile, no SSR
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = false;
				}
			}
		}
		else // weapon not using SSR
		{
			// client, locally controlled - spawn non-replicated projectile, use SSR
			//if (InstigatorPawn->HasAuthority())
			if (HasAuthority())
			{
				//UE_LOG(LogTemp, Display, TEXT("Not SSR, Authority"));
				SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
				//SpawnedProjectile = World->SpawnActorDeferred<AProjectile>(ProjectileClass, SocketTransform);

				SpawnedProjectile->bUseServerSideRewind = false;
				SpawnedProjectile->Damage = Damage;
			}
		}

	}
}
