#include "Goal.h"
#include "ContentsEnum.h"
#include "ContentCore.h"
#include "StageLevel.h"
#include "Mario.h"
#include "Particle.h"

Goal::Goal()
{
}

Goal::~Goal() 
{
}

void Goal::SetGoal(const float4& _Pos)
{
	SetPos(_Pos);
	BarActor = GetLevel()->CreateActor<ClearBar>()->SetGoalActor(this);
}

void Goal::Start()
{
	GoalRender = CreateRender("Goal.bmp", RenderOrder::BackGround);
	GoalRender->SetScaleToImage();
	GoalRender->SetPosition(float4::UP * GoalRender->GetScale().half().Y);

	Collision = CreateCollision(CollisionOrder::Check);
	Collision->SetScale(CollisionScale);
	Collision->SetPosition(CollisionPos);
	Collision->SetDebugRenderType(Rect);
}

void Goal::Update(float _DeltaTime)
{
	if (true == IsClear)
	{
		return;
	}

	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Player), .TargetColType = Rect, .ThisColType = Rect, };
	if (true == Collision->Collision(Check))
	{
		GoalEvent(0);
	}

}

void Goal::Render(float _DeltaTime)
{
	if (true == ContentCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}

void Goal::GoalEvent(int _Score)
{
	dynamic_cast<StageLevel*>(GetLevel())->GoalEvent(_Score);
	IsClear = true;
	Particle::CreateParticle(GetLevel(), BarActor->GetPos(), "SMOKE1");
	BarActor->Death();
}

GameEngineActor* ClearBar::SetGoalActor(Goal* _Goal)
{
	GoalActor = _Goal;
	float4 Pos = GoalActor->GetPos();
	Pos.X -= 16;
	SetPos(Pos);
	UpPos = Pos + float4::UP * Height;
	DownPos = Pos;

	return this;
}

void ClearBar::Start()
{
	BarRender = CreateRender("ClearBar.BMP", RenderOrder::Block);
	BarRender->SetScaleToImage();

	Collision = CreateCollision(CollisionOrder::Check);
	Collision->SetScale(CollisionScale);
	Collision->SetDebugRenderType(Rect);
}

void ClearBar::Update(float _DeltaTime)
{
	switch (State)
	{
	case BarState::Up:
		UpUpdate(_DeltaTime);
		break;
	case BarState::Down:
		DownUpdate(_DeltaTime);
		break;
	default:
		break;
	}

	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Player), .TargetColType = Rect,  .ThisColType = Rect };
	if (Collision->Collision(Check))
	{
		GoalActor->GoalEvent(static_cast<int>(Timer * HeightScore));
	}
}

void ClearBar::Render(float _DeltaTime)
{
	if (true == ContentCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}

void ClearBar::UpUpdate(float _DeltaTime)
{
	Timer += _DeltaTime * Speed;
	if (1 < Timer)
	{
		State = BarState::Down;
		Timer = 1;
		return;
	}
	SetPos(float4::LerpClamp(DownPos, UpPos, Timer));
}

void ClearBar::DownUpdate(float _DeltaTime)
{
	Timer -= _DeltaTime * Speed;
	if (0 > Timer)
	{
		State = BarState::Up;
		Timer = 0;
		return;
	}
	SetPos(float4::LerpClamp(DownPos, UpPos, Timer));
}
