#include "Mario.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnum.h"
#include "Particle.h"
#include "StageLevel.h"
#include "Map.h"
#include "LevelLoader.h"


void Mario::ChangeState(MarioState _State)
{
	MarioState NextState = _State;
	MarioState PrevState = StateValue;
	StateValue = _State;

	if (MarioState::CHANGEPOWER == StateValue)
	{
		ChangePowerStart(PrevState);
		return;
	}
	if (MarioState::GameOver == StateValue)
	{
		GameOverStart();
		return;
	}
	switch (StateValue)
	{
	case MarioState::IDLE:
		IdleEnd();
		break;
	case MarioState::WALK:
		WalkEnd();
		break;
	case MarioState::RUN:
		RunEnd();
		break;
	case MarioState::JUMP:
		JumpEnd();
		break;
	case MarioState::SPIN:
		SpinEnd();
		break;
	case MarioState::BRAKE:
		BrakeEnd();
		break;
	case MarioState::CROUCH:
		CrouchEnd();
		break;
	case MarioState::LOOKUP:
		LookUpEnd();
		break;
	case MarioState::RUNJUMP:
		RunJumpEnd();
		break;
	case MarioState::FALL:
		FallEnd();
		break;
	case MarioState::SLIDE:
		SlideEnd();
		break;
	case MarioState::KICK:
		break;
	case MarioState::CHANGEPOWER:
		ChangePowerEnd();
		break;
	case MarioState::VICTORY:
		break;
	case MarioState::GameOver:
		GameOverEnd();
		break;
	default:
		break;
	}

	switch (NextState)
	{
	case MarioState::IDLE:
		IdleStart();
		break;
	case MarioState::WALK:
		WalkStart();
		break;
	case MarioState::RUN:
		RunStart();
		break;
	case MarioState::JUMP:
		JumpStart();
		break;
	case MarioState::SPIN:
		SpinStart();
		break;
	case MarioState::BRAKE:
		BrakeStart();
		break;
	case MarioState::CROUCH:
		CrouchStart();
		break;
	case MarioState::LOOKUP:
		LookUpStart();
		break;
	case MarioState::RUNJUMP:
		RunJumpStart();
		break;
	case MarioState::FALL:
		FallStart();
		break;
	case MarioState::SLIDE:
		SlideStart();
		break;
	case MarioState::KICK:
		break;
	case MarioState::VICTORY:
		break;
	case MarioState::GameOver:
		GameOverStart();
		break;
	default:
		break;
	}

}

void Mario::UpdateState(float _DeltaTime)
{
	switch (StateValue)
	{
	case MarioState::IDLE:
		IdleUpdate(_DeltaTime);
		break;
	case MarioState::WALK:
		WalkUpdate(_DeltaTime);
		break;
	case MarioState::RUN:
		RunUpdate(_DeltaTime);
		break;
	case MarioState::JUMP:
		JumpUpdate(_DeltaTime);
		break;
	case MarioState::SPIN:
		SpinUpdate(_DeltaTime);
		break;
	case MarioState::BRAKE:
		BrakeUpdate(_DeltaTime);
		break;
	case MarioState::CROUCH:
		CrouchUpdate(_DeltaTime);
		break;
	case MarioState::LOOKUP:
		LookUpUpdate(_DeltaTime);
		break;
	case MarioState::RUNJUMP:
		RunJumpUpdate(_DeltaTime);
		break;
	case MarioState::FALL:
		FallUpdate(_DeltaTime);
		break;
	case MarioState::SLIDE:
		SlideUpdate(_DeltaTime);
		break;
	case MarioState::KICK:
		break;
	case MarioState::CHANGEPOWER:
		ChangePowerUpdate(_DeltaTime);
		break;
	case MarioState::VICTORY:
		break;
	case MarioState::GameOver:
		GameOverUpdate(_DeltaTime);
		break;
	case MarioState::Pipe:
		PipeUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void Mario::IdleStart()
{
	HorizontalForce = 0;
}

void Mario::IdleUpdate(float _DeltaTime)
{
	// ��� Ű�� �Է��� ������
	if (PowerState::Fire == MarioPower && GameEngineInput::IsDown("Dash"))
	{
		ChangeAnimation("JumpAttack");
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Jump"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::JUMP);
		return;
	}
	// ���� Ű�� �Է��� ���
	else if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::SPIN);
		return;
	}
	// �Ʒ� ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Down"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::CROUCH);
		return;
	}
	// ���� ����Ű�� ���ÿ� �Է��ϰų� �Ѵ� �Է����� ���� ���
	else if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (false == GameEngineInput::IsPress("Left") && false == GameEngineInput::IsPress("Right")))
	{
		// ��Ż�濡 �ִ� ���
		if (true == IsSlope)
		{
			ChangeState(MarioState::WALK);
			return;
		}
		// ���� �Է��� ���
		else if (GameEngineInput::IsPress("Up"))
		{
			ChangeState(MarioState::LOOKUP);
			return;
		}
	}
	// ���� ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Left"))
	{
		if (Black == ColMap->GetPixelColor(GetPos() + float4::LEFT * 4, Black))
		{
			return;
		}
		// �������� ������ȯ �� �ȱ���·� ��ȯ
		DirValue = Dir::Left;
		ChangeState(MarioState::WALK);
		return;
	}
	// ������ ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		if (Black == ColMap->GetPixelColor(GetPos() + float4::RIGHT * 4, Black))
		{
			return;
		}
		// ���������� ������ȯ �� �ȱ���·� ��ȯ
		DirValue = Dir::Right;
		ChangeState(MarioState::WALK);
		return;
	}
	ChangeAnimation("Idle");
}

void Mario::IdleEnd()
{
}

void Mario::WalkStart()
{
}

void Mario::WalkUpdate(float _DeltaTime)
{
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Jump"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::JUMP);
		return;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::SPIN);
		return;
	}
	// �Ʒ� ����⸦ �Է��� ���
	if (GameEngineInput::IsPress("Down"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::CROUCH);
		return;
	}


	// ���Է� (����Ű�� �Է����� �ʴ°�� or ���� ����Ű�� ���ÿ� �Է��� ���)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// ��Ż�濡 �ִ� ��Ȳ�� �ƴ� ���
		if (false == IsSlope)
		{
			// �������� ���������� �̵��ϰ� �ִ� ���
			if (0.1f < HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
			}
			// �������� �������� �̵��ϰ� �ִ� ���
			else if (-0.1f > HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
			}
			// �����ӵ� ���� (������)
			else
			{
				// IDLE ���·� ��ȯ
				ChangeState(MarioState::IDLE);
				return;
			}
		}
	}
	// ���� ����Ű�� �������
	else if (GameEngineInput::IsPress("Left"))
	{
		// �������� ������ ������ �����ִ� ���
		if (Dir::Right == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Left;
			// �������� ���������� �̵��ϰ� �ִ� ���
			if (0.1 < HorizontalForce)
			{
				// �극��ũ ���·� ����
				ChangeState(MarioState::BRAKE);
				return;
			}
			RunChargeTime = 0;
		}
		// ��� Ű�� ������ �ִ� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ���ó��
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
			RunChargeTime += _DeltaTime;	// ��ø� �� �ð� ���
		}
		// ��� Ű�� ������ �ʴ� ��� (�ȱ�)
		else
		{
			// �ȱ�ó��

			// ������ ���
			if (true == IsSlope)
			{
				// �ȱ� ó��
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -1.0f);
			}
			// �������� ��÷� �̵��߿���
			else if (-0.5f > HorizontalForce)
			{
				// ������ ������
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȴ� �ӵ� ����
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
			RunChargeTime = 0;
		}
	}
	// ������ ����Ű�� ���� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		// �������� ���� ������ �� ���
		if (Dir::Left == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Right;
			// �������� ���� �������� �̵��� ���
			if (-0.1 > HorizontalForce)
			{
				// �극��ũ ���·� ��ȯ
				ChangeState(MarioState::BRAKE);
				return;
			}
			RunChargeTime = 0;
		}

		// ���Ű�� ���� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ��� ó��
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
			RunChargeTime += _DeltaTime;	// ��ø� �� �ð� ���
		}
		// ��� Ű�� ������ ���� ��� (�Ϲ� �ȱ� ó��)
		else
		{
			// ������ ���
			if (true == IsSlope)
			{
				// �ȱ� ó��
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 1.0f);
			}
			// �������� ��ø� �� ���
			else if (0.5f < HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȱ� ó��
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
			RunChargeTime = 0;
		}
	}

	if (true == IsSlope)
	{
		// �������� �̲������� ���
		if (Dir::Left == SlopeDir)
		{
			// ���������� �̵����� ���
			if (0 < HorizontalForce)
			{
				if (GameEngineInput::IsPress("Dash"))
				{
					HorizontalForce = std::min<float>(HorizontalForce - _DeltaTime, 0.8f);
				}
				else
				{
					HorizontalForce = std::min<float>(HorizontalForce - _DeltaTime, 0.4f);
				}
			}
			// �������� �̵����� ���
			else
			{
				HorizontalForce = std::max<float>(HorizontalForce - _DeltaTime, -1.0f);
			}
		}
		// ���������� �̲������� ���
		else
		{
			// �������� �̵����� ���
			if (0 > HorizontalForce)
			{
				if (GameEngineInput::IsPress("Dash"))
				{
					HorizontalForce = std::max<float>(HorizontalForce + _DeltaTime, -0.8f);
				}
				else
				{
					HorizontalForce = std::max<float>(HorizontalForce + _DeltaTime, -0.4f);
				}
			}
			// ���������� �̵����� ���
			else
			{
				HorizontalForce = std::min<float>(HorizontalForce + _DeltaTime, 1.0f);
			}
		}
	}

	// �ӵ��� 0.7�̻��� ���
	if (0.7f < std::abs(HorizontalForce))
	{
		// ��ø� �Է��� �ð��� 1�ʸ� �ѱ� ���
		if (1 < RunChargeTime)
		{
			// �� ���·� ��ȯ
			ChangeState(MarioState::RUN);
			return;
		}
		ChangeAnimation("Dash");
	}
	else
	{
		ChangeAnimation("Walk");
	}

}

void Mario::WalkEnd()
{
	RunChargeTime = 0;
}

void Mario::RunStart()
{
	HorizontalForce = DirValue == Dir::Right ? 1.0f : -1.0f;
}

void Mario::RunUpdate(float _DeltaTime)
{
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Jump"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::RUNJUMP);
		return;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::SPIN);
		return;
	}
	// �Ʒ� ����⸦ �Է��� ���
	if (GameEngineInput::IsPress("Down"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::CROUCH);
		return;
	}
	// ����Ű�� �Է����� �ʴ°�� or ���� ����Ű�� ���ÿ� �Է��� ���
	else if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// �ȱ���·� ��ȯ
		ChangeState(MarioState::WALK);
		return;
	}
	// ���� ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Left"))
	{
		// �������� ������ ������ ���� �ִ� ���
		if (Dir::Right == DirValue)
		{
			// �������� ������ȯ �� �극��ũ�� ���� ��ȯ
			DirValue = Dir::Left;
			ChangeState(MarioState::BRAKE);
			return;
		}
	}
	// ������ ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		// �������� ���ʹ����� ���� �ִ� ���
		if (Dir::Left == DirValue)
		{
			// ���������� ������ȯ �� �극��ũ�� ������ȯ
			DirValue = Dir::Right;
			ChangeState(MarioState::BRAKE);
			return;
		}
	}
	// ��� Ű �Է��� �� ���
	if (false == GameEngineInput::IsPress("Dash"))
	{
		// �ȱ�� ���� ��ȯ
		ChangeState(MarioState::WALK);
		return;
	}

	if (true == IsSlope)
	{
		// �������� �̲������� ���
		if (Dir::Left == SlopeDir)
		{
			// ���������� �̵����� ���
			if (0 < HorizontalForce)
			{
				HorizontalForce = 0.8f;
			}
			// �������� �̵����� ���
			else
			{
				HorizontalForce = -1.0f;
			}
		}
		// ���������� �̲������� ���
		else
		{
			// �������� �̵����� ���
			if (0 > HorizontalForce)
			{
				HorizontalForce = -0.8f;
			}
			// ���������� �̵����� ���
			else
			{
				HorizontalForce = 1.0f;
			}
		}
	}
	else
	{
		// ���������� �̵����� ���
		if (0 < HorizontalForce)
		{
			HorizontalForce = 1.0f;
		}
		// �������� �̵����� ���
		else
		{
			HorizontalForce = -1.0f;
		}
	}

	ChangeAnimation("Run");

}

void Mario::RunEnd()
{
}

void Mario::BrakeStart()
{
	ParticleDelayTimer = 0;
}

void Mario::BrakeUpdate(float _DeltaTime)
{
	ParticleDelayTimer -= _DeltaTime;
	if (0 > ParticleDelayTimer)
	{
		Particle::CreateParticle(GetLevel(), GetPos(), "SKID");
		ParticleDelayTimer = ParticleDelay;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Jump"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::JUMP);
		return;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::SPIN);
		return;
	}
	// �Ʒ� ����⸦ �Է��� ���
	if (GameEngineInput::IsPress("Down"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::CROUCH);
		return;
	}
	// �¿�Ű�� ���ÿ� �����ų� �¿�Ű �Ѵ� ������ ���� ���
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// IDLE ���·� ��ȯ
		ChangeState(MarioState::WALK);
		return;
	}
	// ���� ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Left"))
	{
		// �������� �������� �����ִ� ���
		if (Dir::Right == DirValue)
		{
			// �������� ������ ��ȯ �� �ȱ� ���·� ��ȯ
			DirValue = Dir::Left;
			ChangeState(MarioState::WALK);
			return;
		}
		// �������� ���������� �̵����� ���
		if (0.1f < HorizontalForce)
		{
			// �ӵ��� ���� ����
			HorizontalForce = std::max<float>(HorizontalForce - (BrakingForce * _DeltaTime), 0);
		}
		// �������� �������� �̵����� ���
		else if (-0.1f > HorizontalForce)
		{
			// �ӵ��� ���� ����
			HorizontalForce = std::min<float>(HorizontalForce + (BrakingForce * _DeltaTime), 0);
		}
		// ���� ���� ���
		else
		{
			// IDLE ���·� ��ȯ
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// ������ ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		// �������� ������ �� ���
		if (Dir::Left == DirValue)
		{
			// ���������� ���� ��ȯ �� �ȱ� ���·� ��ȯ
			DirValue = Dir::Right;
			ChangeState(MarioState::WALK);
			return;
		}
		// �������� ���������� �̵����� ���
		if (0.1f < HorizontalForce)
		{
			// �ӵ��� ����
			HorizontalForce = std::max<float>(HorizontalForce - (BrakingForce * _DeltaTime), 0);
		}
		// �������� �������� �̵����� ���
		else if (-0.1f > HorizontalForce)
		{
			// �ӵ��� ����
			HorizontalForce = std::min<float>(HorizontalForce + (BrakingForce * _DeltaTime), 0);
		}
		// ���� ������ ���
		else
		{
			// IDLE ���·� ��ȯ
			ChangeState(MarioState::IDLE);
			return;
		}
	}

	ChangeAnimation("Brake");
}

void Mario::BrakeEnd()
{
}

void Mario::JumpStart()
{
	IsGrounded = false;

	JumpTimeCounter = JumpTime;
	if (0.5f < std::abs(HorizontalForce))
	{
		MoveDir += float4::UP * DashJumpForce;
	}
	else
	{
		MoveDir += float4::UP * JumpForce;
	}

	GameEngineResources::GetInst().SoundPlay("jump.wav");
}

void Mario::JumpUpdate(float _DeltaTime)
{
	if (GameEngineInput::IsUp("Jump"))
	{
		JumpTimeCounter = 0;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsPress("Jump"))
	{
		if (0 < JumpTimeCounter)
		{
			MoveDir += float4::UP * JumpPressForce * _DeltaTime;
			JumpTimeCounter -= _DeltaTime;
		}
	}
	// ���Է� (����Ű�� �Է����� �ʴ°�� or ���� ����Ű�� ���ÿ� �Է��� ���)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// �������� ���������� �̵��ϰ� �ִ� ���
		if (0.1f < HorizontalForce)
		{
			// ������ �ӵ��� ����
			HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
		}
		// �������� �������� �̵��ϰ� �ִ� ���
		else if (-0.1f > HorizontalForce)
		{
			// ������ �ӵ��� ����
			HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
		}
		// �����ӵ� ���� (������)
		else
		{
			HorizontalForce = 0;
		}
	}
	// ���� ����Ű�� �������
	else if (GameEngineInput::IsPress("Left"))
	{
		// �������� ������ ������ �����ִ� ���
		if (Dir::Right == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Left;
			ChangeAnimation("Jump");
		}
		// ��� Ű�� ������ �ִ� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ���ó��
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
		}
		// ��� Ű�� ������ �ʴ� ��� (�ȱ�)
		else
		{
			// �ȱ�ó��

			// �������� ��÷� �̵��߿���
			if (-0.5f > HorizontalForce)
			{
				// ������ ������
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȴ� �ӵ� ����
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
		}
	}
	// ������ ����Ű�� ���� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		// �������� ���� ������ �� ���
		if (Dir::Left == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Right;
			ChangeAnimation("Jump");
		}

		// ���Ű�� ���� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ��� ó��
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
		}
		// ��� Ű�� ������ ���� ��� (�Ϲ� �ȱ� ó��)
		else
		{
			// �������� ��ø� �� ���
			if (0.5f < HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȱ� ó��
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
		}
	}

	ChangeAnimation("Jump");
}

void Mario::JumpEnd()
{
}

void Mario::SpinStart()
{
	IsGrounded = false;

	MoveDir += float4::UP * JumpForce;
	JumpTimeCounter = JumpTime;
	GameEngineResources::GetInst().SoundPlay("spin.wav");
}

void Mario::SpinUpdate(float _DeltaTime)
{
	if (GameEngineInput::IsUp("Spin"))
	{
		JumpTimeCounter = 0;
	}
	if (true == IsGrounded)
	{
		if (0 < std::abs(HorizontalForce))
		{
			ChangeState(MarioState::WALK);
			return;
		}
		else
		{
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsPress("Spin"))
	{
		if (0 < JumpTimeCounter)
		{
			MoveDir += float4::UP * SpinPressForce * _DeltaTime;
			JumpTimeCounter -= _DeltaTime;
		}
	}
	// ���Է� (����Ű�� �Է����� �ʴ°�� or ���� ����Ű�� ���ÿ� �Է��� ���)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// �������� ���������� �̵��ϰ� �ִ� ���
		if (0.1f < HorizontalForce)
		{
			// ������ �ӵ��� ����
			HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
		}
		// �������� �������� �̵��ϰ� �ִ� ���
		else if (-0.1f > HorizontalForce)
		{
			// ������ �ӵ��� ����
			HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
		}
		// �����ӵ� ���� (������)
		else
		{
			HorizontalForce = 0;
		}
	}
	// ���� ����Ű�� �������
	else if (GameEngineInput::IsPress("Left"))
	{
		// �������� ������ ������ �����ִ� ���
		if (Dir::Right == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Left;
		}
		// ��� Ű�� ������ �ִ� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ���ó��
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
		}
		// ��� Ű�� ������ �ʴ� ��� (�ȱ�)
		else
		{
			// �ȱ�ó��

			// �������� ��÷� �̵��߿���
			if (-0.5f > HorizontalForce)
			{
				// ������ ������
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȴ� �ӵ� ����
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
		}
	}
	// ������ ����Ű�� ���� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		// �������� ���� ������ �� ���
		if (Dir::Left == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Right;
		}

		// ���Ű�� ���� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ��� ó��
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
		}
		// ��� Ű�� ������ ���� ��� (�Ϲ� �ȱ� ó��)
		else
		{
			// �������� ��ø� �� ���
			if (0.5f < HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȱ� ó��
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
		}
	}
	ChangeAnimation("Spin");
}

void Mario::SpinEnd()
{
}

void Mario::CrouchStart()
{

}

void Mario::CrouchUpdate(float _DeltaTime)
{

	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Jump"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::JUMP);
		return;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::SPIN);
		return;
	}
	if (false == GameEngineInput::IsPress("Down"))
	{
		if (0.1f < std::abs(HorizontalForce))
		{
			ChangeState(MarioState::WALK);
			return;
		}
		else
		{
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// ������ ���
	if (true == IsSlope && false == IsHold)
	{
		ChangeState(MarioState::SLIDE);
		return;
	}

	// ���� �������� ���������� �̵��ϴ� ���
	if (0.1f < HorizontalForce)
	{
		// �ӵ��� ���� ���δ�
		HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
	}
	// ���� �������� �������� �̵��ϴ� ���
	else if (-0.1f > HorizontalForce)
	{
		// �ӵ��� ���� ���δ�
		HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
	}
	else
	{
		HorizontalForce = 0;
	}

	ChangeAnimation("Crouch");

}

void Mario::CrouchEnd()
{
}

void Mario::LookUpStart()
{

}

void Mario::LookUpUpdate(float _DeltaTime)
{
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Jump"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::JUMP);
		return;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::SPIN);
		return;
	}
	// ���� ����Ű�� �� ���
	if (false == GameEngineInput::IsPress("Up"))
	{
		ChangeState(MarioState::IDLE);
		return;
	}
	// ���� ����Ű�� ���ÿ� �Է��� ���
	else if (GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right"))
	{
		return;
	}
	// ���� ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Left"))
	{
		// �������� ������ȯ �� �ȱ���·� ��ȯ
		DirValue = Dir::Left;
		ChangeState(MarioState::WALK);
		return;
	}
	// ������ ����Ű�� �Է��� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		// ���������� ������ȯ �� �ȱ���·� ��ȯ
		DirValue = Dir::Right;
		ChangeState(MarioState::WALK);
		return;
	}
	ChangeAnimation("LookUp");
}

void Mario::LookUpEnd()
{
}

void Mario::RunJumpStart()
{
	IsGrounded = false;
	JumpTimeCounter = JumpTime;
	MoveDir += float4::UP * RunJumpForce;
	GameEngineResources::GetInst().SoundPlay("jump.wav");
}

void Mario::RunJumpUpdate(float _DeltaTime)
{
	if (GameEngineInput::IsUp("Jump"))
	{
		JumpTimeCounter = 0;
	}
	if (true == IsGrounded)
	{
		if ((Dir::Right == DirValue && 0.5f < HorizontalForce) || (Dir::Left == DirValue && -0.5f > HorizontalForce))
		{
			ChangeState(MarioState::RUN);
			return;
		}
		else if (0 < std::abs(HorizontalForce))
		{
			ChangeState(MarioState::WALK);
			return;
		}
		else
		{
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsPress("Jump"))
	{
		if (0 < JumpTimeCounter)
		{
			MoveDir += float4::UP * JumpPressForce * _DeltaTime;
			JumpTimeCounter -= _DeltaTime;
		}
	}
	// ���Է� (����Ű�� �Է����� �ʴ°�� or ���� ����Ű�� ���ÿ� �Է��� ���)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// �������� ���������� �̵��ϰ� �ִ� ���
		if (0.1f < HorizontalForce)
		{
			// ������ �ӵ��� ����
			HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
		}
		// �������� �������� �̵��ϰ� �ִ� ���
		else if (-0.1f > HorizontalForce)
		{
			// ������ �ӵ��� ����
			HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
		}
		// �����ӵ� ���� (������)
		else
		{
			HorizontalForce = 0;
		}
	}
	// ���� ����Ű�� �������
	else if (GameEngineInput::IsPress("Left"))
	{
		// �������� ������ ������ �����ִ� ���
		if (Dir::Right == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Left;
			ChangeAnimation("RunJump");
		}
		// ��� Ű�� ������ �ִ� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ���ó��
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
		}
		// ��� Ű�� ������ �ʴ� ��� (�ȱ�)
		else
		{
			// �ȱ�ó��

			// �������� ��÷� �̵��߿���
			if (-0.5f > HorizontalForce)
			{
				// ������ ������
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȴ� �ӵ� ����
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
		}
	}
	// ������ ����Ű�� ���� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		// �������� ���� ������ �� ���
		if (Dir::Left == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Right;
			ChangeAnimation("RunJump");
		}

		// ���Ű�� ���� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ��� ó��
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
		}
		// ��� Ű�� ������ ���� ��� (�Ϲ� �ȱ� ó��)
		else
		{
			// �������� ��ø� �� ���
			if (0.5f < HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȱ� ó��
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
		}
	}

	ChangeAnimation("RunJump");
}

void Mario::RunJumpEnd()
{
}

void Mario::FallStart()
{
}

void Mario::FallUpdate(float _DeltaTime)
{

	if (true == IsGrounded)
	{
		if (0 < std::abs(HorizontalForce))
		{
			if (-0.1f > HorizontalForce && true == GameEngineInput::IsPress("Right"))
			{
				ChangeState(MarioState::BRAKE);
				return;
			}
			if (0.1f < HorizontalForce && true == GameEngineInput::IsPress("Left"))
			{
				ChangeState(MarioState::BRAKE);
				return;
			}
			ChangeState(MarioState::WALK);
			return;
		}
		else
		{
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsPress("Jump"))
	{
	}
	// ���Է� (����Ű�� �Է����� �ʴ°�� or ���� ����Ű�� ���ÿ� �Է��� ���)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// �������� ���������� �̵��ϰ� �ִ� ���
		if (0.1f < HorizontalForce)
		{
			// ������ �ӵ��� ����
			HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
		}
		// �������� �������� �̵��ϰ� �ִ� ���
		else if (-0.1f > HorizontalForce)
		{
			// ������ �ӵ��� ����
			HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
		}
		// �����ӵ� ���� (������)
		else
		{
			HorizontalForce = 0;
		}
	}
	// ���� ����Ű�� �������
	else if (GameEngineInput::IsPress("Left"))
	{
		// �������� ������ ������ �����ִ� ���
		if (Dir::Right == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Left;
			ChangeAnimation("Fall");
		}
		// ��� Ű�� ������ �ִ� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ���ó��
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
		}
		// ��� Ű�� ������ �ʴ� ��� (�ȱ�)
		else
		{
			// �ȱ�ó��

			// �������� ��÷� �̵��߿���
			if (-0.5f > HorizontalForce)
			{
				// ������ ������
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȴ� �ӵ� ����
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
		}
	}
	// ������ ����Ű�� ���� ���
	else if (GameEngineInput::IsPress("Right"))
	{
		// �������� ���� ������ �� ���
		if (Dir::Left == DirValue)
		{
			// ������ȯ
			DirValue = Dir::Right;
			ChangeAnimation("Fall");
		}

		// ���Ű�� ���� ���
		if (GameEngineInput::IsPress("Dash"))
		{
			// ��� ó��
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
		}
		// ��� Ű�� ������ ���� ��� (�Ϲ� �ȱ� ó��)
		else
		{
			// �������� ��ø� �� ���
			if (0.5f < HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// �׿� ��� (�Ϲ����� �ȱ�)
			else
			{
				// �ȱ� ó��
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
		}
	}

	ChangeAnimation("Fall");
}

void Mario::FallEnd()
{
}

void Mario::SlideStart()
{
	ParticleDelayTimer = 0;
}

void Mario::SlideUpdate(float _DeltaTime)
{
	ParticleDelayTimer -= _DeltaTime;
	if (0 > ParticleDelayTimer)
	{
		Particle::CreateParticle(GetLevel(), GetPos(), "SKID");
		ParticleDelayTimer = ParticleDelay;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Jump"))
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::JUMP);
		return;
	}
	// ���� Ű�� �Է��� ���
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// ���� ���·� ��ȯ
		ChangeState(MarioState::SPIN);
		return;
	}
	if (false == GameEngineInput::IsPress("Down"))
	{
		ChangeState(MarioState::WALK);
		return;
	}
	if (false == IsSlope)
	{
		if (0 != std::abs(HorizontalForce))
		{
			// �������� ���������� �̵��ϰ� �ִ� ���
			if (0.1f < HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
			}
			// �������� �������� �̵��ϰ� �ִ� ���
			else if (-0.1f > HorizontalForce)
			{
				// ������ �ӵ��� ����
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
			}
			// �����ӵ� ���� (������)
			else
			{
				// IDLE ���·� ��ȯ
				ChangeState(MarioState::CROUCH);
			}
		}
		else
		{
			ChangeState(MarioState::CROUCH);
		}
		return;
	}

	// �������� �̲������� ���
	if (Dir::Left == SlopeDir)
	{
		// ���������� �̵����� ���
		if (0 < HorizontalForce)
		{
			HorizontalForce = HorizontalForce - DashAcceleration * _DeltaTime;
		}
		// �������� �̵����� ���
		else
		{
			HorizontalForce = std::max<float>(HorizontalForce - DashAcceleration * _DeltaTime, -1.2f);
		}
	}
	// ���������� �̲������� ���
	else
	{
		// �������� �̵����� ���
		if (0 > HorizontalForce)
		{
			HorizontalForce = HorizontalForce + DashAcceleration * _DeltaTime;
		}
		// ���������� �̵����� ���
		else
		{
			HorizontalForce = std::min<float>(HorizontalForce + DashAcceleration * _DeltaTime, 1.2f);
		}
	}

	ChangeAnimation("Slide");
}

void Mario::SlideEnd()
{
}

void Mario::ChangePowerStart(MarioState _BeforeState)
{
	CurrentLevel->LevelPause();
	BeforeState = _BeforeState;

	std::string AnimStr = DirValue == Dir::Left ? "Left_" : "Right_";
	Timer = ChangePowerTime;
	AnimationRender->On();

	if (PowerState::Super == MarioPower)
	{
		AnimationRender->ChangeAnimation(AnimStr + "Grow");
		return;
	}
	if (PowerState::Normal == MarioPower)
	{
		AnimationRender->ChangeAnimation(AnimStr + "Shrink");
	}
	if (PowerState::Fire == MarioPower)
	{
		AnimationRender->ChangeAnimation(AnimStr + "GetFire");
	}
}

void Mario::ChangePowerUpdate(float _DeltaTime)
{
	Timer -= _DeltaTime;
	if (0 >= Timer) {
		if (PowerState::Normal == MarioPower)
		{
			CurrentLevel->DropStockItem();
			Timer = InvincibilityTime;
		}
		switch (BeforeState)
		{
		case MarioState::IDLE:
			ChangeAnimation("Idle");
			break;
		case MarioState::WALK:
			ChangeAnimation("Walk");
			break;
		case MarioState::RUN:
			ChangeAnimation("RUN");
			break;
		case MarioState::BRAKE:
			ChangeAnimation("BRAKE");
			break;
		case MarioState::CROUCH:
			ChangeAnimation("CROUCH");
			break;
		case MarioState::LOOKUP:
			ChangeAnimation("LOOKUP");
			break;
		case MarioState::JUMP:
			ChangeAnimation("JUMP");
			break;
		case MarioState::FALL:
			ChangeAnimation("FALL");
			break;
		case MarioState::RUNJUMP:
			ChangeAnimation("RUNJUMP");
			break;
		case MarioState::SPIN:
			ChangeAnimation("SPIN");
			break;
		case MarioState::SLIDE:
			ChangeAnimation("SLIDE");
			break;
		case MarioState::KICK:
			ChangeAnimation("KICK");
			break;
		default:
			ChangeState(MarioState::FALL);
			return;
		}
		StateValue = BeforeState;
		ChangePowerEnd();
	}
}

void Mario::ChangePowerEnd()
{
	CurrentLevel->LevelPlay();
}

void Mario::GameOverStart()
{
	CurrentLevel->LevelPause();
	AnimationRender->ChangeAnimation("GameOver1");
	Timer = 0;
	MoveDir = float4::ZERO;
	CurrentLevel->GetBGMPlayer().Stop();
	GameEngineResources::GetInst().SoundPlay("PlayerDown.mp3");
}

void Mario::GameOverUpdate(float _DeltaTime)
{
	if (true == IsDie) { return; }
	Timer += _DeltaTime;

	if (3.0f < Timer)
	{
		CurrentLevel->MarioDie();
		IsDie = true;
	}
	else if (0.8f < Timer)
	{
		MoveDir.Y -= _DeltaTime;
	}
	else if (0.5f < Timer)
	{
		AnimationRender->ChangeAnimation("GameOver2");
		MoveDir.Y += _DeltaTime;
	}

	if (1 < MoveDir.Y)
	{
		MoveDir.Y = 1;
	}
	SetMove(float4::UP * 2000 * MoveDir.Y * _DeltaTime);
}

void Mario::GameOverEnd()
{
}

void Mario::VictoryUpdate(float _DeltaTime)
{
	if (Timer < 3)
	{
		SetMove(float4::RIGHT * ClearWalkSpeed * _DeltaTime);
	}
	else if (7.2 < Timer)
	{
		AnimationRender->ChangeAnimation(BeforeAnim.data() + std::string("Goal"));
	}
	else if (4 < Timer)
	{
		ChangeAnimation("Idle");
	}
	Timer += _DeltaTime;


	// �� �浹 üũ

	if (nullptr == ColMap)
	{
		MsgAssert("�浹�� �� �̹����� �����ϴ�.");
	}

	float4 DownPos = GetPos() + float4::DOWN * 10;
	// �� �浹 üũ�� �÷� ����
	DWORD PixelColor = ColMap->GetPixelColor(DownPos, White);
	if (Black == PixelColor)
	{
		DownPos.Y = std::round(DownPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			DownPos.Y -= 1;
			PixelColor = ColMap->GetPixelColor(DownPos, Black);
			if (Black != PixelColor)
			{
				SetPos(DownPos);
				MoveDir.Y = 0.0f;
				break;
			}
		}
	}
	else
	{
		MoveDir.Y += GravityAcceleration * _DeltaTime;
		if (GravityMax < MoveDir.Y)
		{
			MoveDir.Y = GravityMax;
		}
		SetMove(float4::DOWN * MoveDir.Y * _DeltaTime);
	}

}

void Mario::PipeStart()
{
}

void Mario::PipeUpdate(float _DeltaTime)
{
	Timer -= _DeltaTime;
	if (0 > Timer)
	{
		ChangeState(MarioState::IDLE);
		AnimationRender->SetOrder(static_cast<int>(RenderOrder::Player));
	}
	SetMove(MoveDir * _DeltaTime * 192);
}