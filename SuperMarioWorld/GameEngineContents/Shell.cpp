#include "Shell.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnum.h"
#include "Map.h"
#include "Block.h"
#include "Particle.h"
#include "Mario.h"
#include "Coin.h"

Shell::Shell() 
{
}

Shell::~Shell()
{
}

bool Shell::IsCollisionAttack()
{
	return StateValue == ShellState::Spin && Timer < 0;
}


bool Shell::IsHold()
{
	return StateValue == ShellState::Hold;
}

bool Shell::IsHoldable()
{
	return StateValue == ShellState::Idle && Timer < 0;
}

bool Shell::IsDie()
{
	return false;
}

void Shell::SpinHit()
{
	GameEngineResources::GetInst().SoundPlay("superstomp.wav");
	Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
	Particle::CreateMovingParticle(GetLevel(), GetPos(), { 450, -450 }, "StarParticle", false, true, 0.5f);
	Death();
}

void Shell::JumpHit(bool IsLeft)
{
	Mario::MainPlayer->AddScore(Score);
	Timer = StateWaitTime;

	if (StateValue == ShellState::Idle)
	{
		StateValue = ShellState::Spin;
		AnimationRender->ChangeAnimation("Spin");

		if (true == IsLeft)
		{
			DirSetting(float4::LEFT);
		}
		else
		{
			DirSetting(float4::RIGHT);
		}
	}
	else
	{
		MoveDir.X = 0;
		StateValue = ShellState::Idle;
		AnimationRender->ChangeAnimation("Idle");
	}
}

void Shell::FireHit()
{
	GetLevel()->CreateActor<Coin>(RenderOrder::Item)->SetPos(GetPos());
	Mario::MainPlayer->AddScore(Score);
	Death();
}

void Shell::BlockHit()
{
}

void Shell::MonsterHit(bool IsLeft)
{
	Mario::MainPlayer->AddScore(Score);
	// 상태 변경
	StateValue = ShellState::Die;
	MoveDir = MonsterHitForce;

	// 이동 지정
	if (true == IsLeft)
	{
		MoveDir.X *= -1;
	}
}

void Shell::Hold()
{
	StateValue = ShellState::Hold;
}

void Shell::Kick(const float4& _Force)
{
	if (-1000 > _Force.Y)
	{
		MoveDir = _Force;
		StateValue = ShellState::Idle;
		Timer = KickWaitTime;
		return;
	}

	if (500 < _Force.X)
	{
		DirSetting(float4::RIGHT);
		StateValue = ShellState::Kick;
		Timer = KickWaitTime;
		AnimationRender->ChangeAnimation("Spin");
	}
	else if (-500 > _Force.X)
	{
		DirSetting(float4::LEFT);
		StateValue = ShellState::Kick;
		Timer = KickWaitTime;
		AnimationRender->ChangeAnimation("Spin");
	}
	else
	{
		StateValue = ShellState::Idle;
		Timer = StateWaitTime;
		return;
	}

}

void Shell::Start()
{
	EnemyActor::Start();
	EnemyActor::LevelChangeStart(nullptr);
	Speed = ActorSpeed;
	SlopeSpeed = ActorSlopeSpeed;

	// 렌더 생성
	{
		AnimationRender = CreateRender(RenderOrder::Monster);
		AnimationRender->SetScale(RenderScale);
		AnimationRender->CreateAnimation({ .AnimationName = "Idle", .ImageName = "SHELL.BMP", .Start = 0, .End = 0, });
		AnimationRender->CreateAnimation({ .AnimationName = "Spin", .ImageName = "SHELL.BMP", .Start = 0, .End = 3, .InterTime = 0.08f });
		AnimationRender->ChangeAnimation("Idle");
		AnimationRender->SetPosition(RenderPos);
	}
	// Collision 생성
	{
		Collision = CreateCollision(CollisionOrder::Monster);
		Collision->SetScale(CollisionScale);
		Collision->SetPosition(CollisionPos);
		Collision->SetDebugRenderType(CollisionType::Rect);
	}
}

void Shell::Update(float _DeltaTime)
{
	CameraInCheck();
	if (false == IsOnCamera)
	{
		if (true == IsOn)
		{
			Death();
		}
		return;
	}
	IsOn = true;
	switch (StateValue)
	{
	case ShellState::Idle:
		IdleUpdate(_DeltaTime);
		MoveUpdate(_DeltaTime);
		break;
	case ShellState::Hold:
		MonsterCheck();
		break;
	case ShellState::Kick:
		KickUpdate(_DeltaTime);
		MonsterCheck();
		break;
	case ShellState::Spin:
		SpinUpdate(_DeltaTime);
		MonsterCheck();
		break;
	case ShellState::Die:
		DieUpdate(_DeltaTime);
		break;
	default:
		break;
	}
	Timer -= _DeltaTime;
}

void Shell::Turn()
{
	EnemyActor::Turn();

	if (StateValue != ShellState::Idle)
	{
		StateValue = ShellState::Spin;
		Timer = 0;
	}
}

void Shell::TurnLeft()
{
	EnemyActor::TurnLeft();
}

void Shell::TurnRight()
{
	EnemyActor::TurnRight();
}

void Shell::IdleUpdate(float _DeltaTime)
{
}

void Shell::KickUpdate(float _DeltaTime)
{
	if (0 > Timer)
	{
		StateValue = ShellState::Spin;
		return;
	}
	SpinUpdate(_DeltaTime);
}

void Shell::SpinUpdate(float _DeltaTime)
{


	// 중력 적용
	MoveDir.Y += GravityAcceleration * _DeltaTime;
	if (GravityMax < MoveDir.Y)
	{
		MoveDir.Y = GravityMax;
	}

	// 충돌 이미지 검사
	if (nullptr == ColMap)
	{
		MsgAssert("충돌용 맵 이미지가 없습니다.");
	}
	// 이동될 위치
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;
	float4 ForwardPos = NextPos;
	ForwardPos.Y = GetPos().Y + float4::UP.Y;

	// 맵 충돌 체크용 컬러 변수
	DWORD PixelColor = ColMap->GetPixelColor(ForwardPos, White);

	// 벽 체크
	if (Black == PixelColor)
	{
		Turn();
		return;
	}
	// 바닥 체크
	PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (Black != PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				//StateValue = State::Overturn;
				return;
			}
		}
	}
	// 아래에서 통과되는 블록들 체크 ex) 구름
	else if (Green == PixelColor && 0 < MoveDir.Y)
	{
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				//StateValue = State::Overturn;
				return;
			}
		}
	}
	// 비탈길 체크
	else if (Red == PixelColor && 0 < MoveDir.Y)
	{
		float4 SlopePos = NextPos;
		SlopePos += SlopeRightUp;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				return;
			}
		}
	}

	// 블록 체크
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Block), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		bool Hit = false;
		for (; Start != End; Start++)
		{
			Block* HitBlock = (*Start)->GetOwner<Block>();
			if (true == HitBlock->GetIsRoll())
			{
				continue;
			}
			// 엑터가 블록보다 위에 있는 경우
			if (GetPos().Y < HitBlock->GetPos().Y - BlockYSize)
			{
				// 블록위에 선다
				float4 Pos = GetPos();
				Pos.Y = HitBlock->GetPos().Y - BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = 0.0f;
				continue;
			}
			// 밑에서 위로 친 경우
			else if (GetPos().Y > HitBlock->GetPos().Y)
			{
				// 블록이 히트한다
				Hit = true;
				HitBlock->Hit();
				// 블록 밑으로 이동한다
				float4 Pos = GetPos();
				Pos.Y = HitBlock->GetPos().Y + BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = HeadingReaction;
			}
			// 그 외 경우 (옆에서 친 경우)
			else
			{
				// 방향에 따라 위치 조정한다
				if (GetPos().X < HitBlock->GetPos().X)
				{
					float4 Pos = GetPos();
					Pos.X = HitBlock->GetPos().X - BlockXSize - RenderScale.hx();
					Pos.X = std::round(Pos.X);
					SetPos(Pos);

				}
				else
				{
					float4 Pos = GetPos();
					Pos.X = HitBlock->GetPos().X + BlockXSize + RenderScale.hx();
					Pos.X = std::round(Pos.X);
					SetPos(Pos);
				}
				// 블록이 히트한다
				Hit = true;
				HitBlock->Hit();
			}

		}
		// 블록이 히트한 경우 효과음 및 좌우 이동 정지
		if (true == Hit)
		{
			Turn();
			GameEngineResources::GetInst().SoundPlay("bump.wav");
			return;
		}
	}
	SetMove(MoveDir * _DeltaTime);

}

void Shell::MonsterCheck()
{
	// 몬스터 체크
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Monster), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		for (; Start != End; Start++)
		{
			EnemyActor* ColActor = (*Start)->GetOwner<EnemyActor>();
			if (this == ColActor)
			{
				continue;
			}
			if (true == ColActor->IsHold())
			{
				continue;
			}
			if (true == ColActor->IsDie())
			{
				continue;
			}
			Mario::MainPlayer->DropHold();
			GameEngineResources::GetInst().SoundPlay("kick.wav");
			Particle::CreateParticle(GetLevel(), GetPos(), "KICK");

			if (ColActor->GetPos().X < GetPos().X)
			{
				ColActor->MonsterHit(true);
				if (StateValue == ShellState::Hold)
				{
					MonsterHit(false);
				}
			}
			else
			{
				ColActor->MonsterHit(false);
				if (StateValue == ShellState::Hold)
				{
					MonsterHit(true);
				}
			}
			return;
		}
	}
}
