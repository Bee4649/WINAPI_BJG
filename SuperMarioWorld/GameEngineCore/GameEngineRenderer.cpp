#include "GameEngineRenderer.h"
#include "GameEngineCore.h"
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineBase/GameEngineString.h>
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include <GameEnginePlatform/GameEngineWindow.h>


GameEngineRenderer::GameEngineRenderer() 
{
}

GameEngineRenderer::~GameEngineRenderer() 
{
	
}

void GameEngineRenderer::SetRotFilter(const std::string_view& _ImageName)
{
	RotationFilter = GameEngineResources::GetInst().TextureFind(_ImageName);
}

void GameEngineRenderer::SetImage(const std::string_view& _ImageName)
{
	Image = GameEngineResources::GetInst().TextureFind(_ImageName);
	
}

void GameEngineRenderer::SetImageToScaleToImage(const std::string_view& _ImageName)
{
	Image = GameEngineResources::GetInst().TextureFind(_ImageName);
	SetScaleToImage();
}

void GameEngineRenderer::SetScaleToImage()
{
	if (nullptr == Image)
	{
		MsgAssert("�̹����� �������� �ʾҴµ� �̹����� ũ��� �����Ϸ��� �߽��ϴ�.");
	}

	SetScale(Image->GetImageScale());
}

void GameEngineRenderer::SetOrder(int _Order)
{
	GetActor()->GetLevel()->PushRender(this, _Order);
}

void GameEngineRenderer::SetFrame(int _Frame)
{
	if (nullptr == Image)
	{
		MsgAssert("�̹����� �������� �ʴ� �������� �������� �����Ϸ��� �߽��ϴ�.");
	}

	if (false == Image->IsImageCutting())
	{
		MsgAssert("�߷��ִ� �̹����� �������� ������ �� �� �ֽ��ϴ�.");
	}

	if (false == Image->IsCutIndexValid(_Frame))
	{
		MsgAssert("�������� ���� �̹����ε��� �Դϴ�.");
	}

	Frame = _Frame;
}

bool GameEngineRenderer::FrameAnimation::IsEnd()
{
	int Value = (static_cast<int>(FrameIndex.size()) - 1);
	return CurrentIndex == Value;
}


void GameEngineRenderer::FrameAnimation::Render(float _DeltaTime)
{
	CurrentTime -= _DeltaTime;

	if (CurrentTime <= 0.0f)
	{
		++CurrentIndex;

		if (FrameIndex.size() <= CurrentIndex)
		{
			if (true == Loop)
			{
				CurrentIndex = 0;
			}
			else {
				CurrentIndex = static_cast<int>(FrameIndex.size()) - 1;
			}
		}

		CurrentTime += FrameTime[CurrentIndex];
	}
}

void GameEngineRenderer::SetText(const std::string_view& _Text, const int _TextHeight, const std::string_view& _TextType, const TextAlign _TextAlign, const COLORREF _TextColor, float4 _TextBoxScale)
{
	RenderText = _Text;
	TextHeight = _TextHeight;
	TextType = _TextType;
	Align = _TextAlign;
	TextColor = _TextColor;
	TextBoxScale = _TextBoxScale;
}

void GameEngineRenderer::Render(float _DeltaTime)
{
	if (RenderText != "")
	{
		TextRender(_DeltaTime);
	}
	else
	{
		ImageRender(_DeltaTime);
	}
}

void GameEngineRenderer::TextRender(float _DeltaTime)
{

	float4 CameraPos = float4::ZERO;

	if (true == IsEffectCamera)
	{
		CameraPos = GetActor()->GetLevel()->GetCameraPos();
	}

	float4 RenderPos = GetActorPlusPos() - CameraPos;

	HDC hdc = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
	HFONT hFont, OldFont;
	LOGFONTA lf;
	lf.lfHeight = TextHeight;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0; // ���ؼ��� ����̽��� x�� ������ ����(10��)
	lf.lfWeight = 0; // ���� ǥ��
	lf.lfItalic = 0; // ����Ӳ� �۲�
	lf.lfUnderline = 0; // ������ ���� �۲�
	lf.lfStrikeOut = 0; // ��Ҽ� �۲�
	lf.lfCharSet = HANGEUL_CHARSET; // ���� ����
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0; // ��� ǰ�� GDI
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN; // ��Ʈ 4 (���� ��Ʈ��ũ �ʺ�(���) �� �������� �ִ� �۲��Դϴ�. MS Serif�� ���� �� �� �ֽ��ϴ�.)
	lstrcpy(lf.lfFaceName, TEXT(TextType.c_str())); // 32 TCHAR ���� �ʰ��ϸ� �ȵ�.
	hFont = CreateFontIndirect(&lf);
	OldFont = static_cast<HFONT>(SelectObject(hdc, hFont));

	SetTextColor(hdc, TextColor);
	SetBkMode(hdc, TRANSPARENT);

	RECT Rect;
	Rect.left = RenderPos.iX();
	Rect.top = RenderPos.iY();
	Rect.right = RenderPos.iX() + TextBoxScale.iY();
	Rect.bottom = RenderPos.iY() + TextBoxScale.iY();

	if (GameEngineCore::GetInst()->IsDebug())
	{
		HDC DoubleDC = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
		HBRUSH myBrush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
		HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(DoubleDC, myBrush));
		HPEN myPen = CreatePen(PS_DASH, 0, RGB(0, 0, 0));
		HPEN oldPen = static_cast<HPEN>(SelectObject(DoubleDC, myPen));
		
		Rectangle(GameEngineWindow::GetDoubleBufferImage()->GetImageDC(),
			Rect.left, Rect.top, Rect.right, Rect.bottom);

		SelectObject(DoubleDC, oldBrush);
		DeleteObject(myBrush);
		SelectObject(DoubleDC, oldPen);
		DeleteObject(myPen);
	}

	DrawTextA(GameEngineWindow::GetDoubleBufferImage()->GetImageDC(),
		RenderText.c_str(), static_cast<int>(RenderText.size()),
		&Rect, static_cast<UINT>(Align));
	
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

	return;
}

void GameEngineRenderer::ImageRender(float _DeltaTime)
{
	if (nullptr != CurrentAnimation)
	{
		CurrentAnimation->Render(_DeltaTime);
		Frame = CurrentAnimation->FrameIndex[CurrentAnimation->CurrentIndex];
		Image = CurrentAnimation->Image;
		RotationFilter = CurrentAnimation->FilterImage;
	}

	if (nullptr == Image)
	{
		MsgAssert("�̹����� ���������� �ʾҽ��ϴ�.");
	}

	float4 CameraPos = float4::ZERO;

	if (true == IsEffectCamera)
	{
		CameraPos = GetActor()->GetLevel()->GetCameraPos();
	}

	float4 RenderPos = GetActorPlusPos() - CameraPos;

	if (true == Image->IsImageCutting())
	{
		if (Angle != 0.0f)
		{
			if (nullptr == RotationFilter)
			{
				MsgAssert("ȸ�� ��ų �� ���� �̹��� �Դϴ�. ���Ͱ� �������� �ʽ��ϴ�.");
			}

			GameEngineWindow::GetDoubleBufferImage()->PlgCopy(Image, Frame, RenderPos, GetScale(), Angle, RotationFilter);
		}
		else if (255 == Alpha)
		{
			GameEngineWindow::GetDoubleBufferImage()->TransCopy(Image, Frame, RenderPos, GetScale(), TransColor);
		}
		else if (255 > Alpha)
		{
			GameEngineWindow::GetDoubleBufferImage()->AlphaCopy(Image, Frame, RenderPos, GetScale(), Alpha);
		}
	}
	else
	{
		if (Angle != 0.0f)
		{
			if (nullptr == RotationFilter)
			{
				MsgAssert("ȸ����ų�� ���� �̹��� �Դϴ�. ���Ͱ� �������� �ʽ��ϴ�.");
			}

			GameEngineWindow::GetDoubleBufferImage()->PlgCopy(Image, Frame, RenderPos, GetScale(), Angle, RotationFilter);
		}
		else if (255 == Alpha)
		{
			GameEngineWindow::GetDoubleBufferImage()->TransCopy(Image, RenderPos, GetScale(), { 0, 0 }, Image->GetImageScale(), TransColor);
		}
		else if (255 > Alpha)
		{
			GameEngineWindow::GetDoubleBufferImage()->AlphaCopy(Image, RenderPos, GetScale(), { 0, 0 }, Image->GetImageScale(), Alpha);
		}
	}
}

bool GameEngineRenderer::IsAnimationEnd()
{
	return CurrentAnimation->IsEnd();
}

void GameEngineRenderer::CreateAnimation(const FrameAnimationParameter& _Paramter)
{
	// �ִϸ��̼��� ����� ���ؼ� �̹����� �����Ѵ�.
	GameEngineWindowTexture* Image = GameEngineResources::GetInst().TextureFind(_Paramter.ImageName);

	if (nullptr == Image)
	{
		MsgAssert("�������� �ʴ� �̹����� �ִϸ��̼��� ������� �߽��ϴ�.");
	}

	if (false == Image->IsImageCutting())
	{
		MsgAssert("�߷��ִ� �̹����� �������� �������� �� �ֽ��ϴ�.");
	}

	std::string UpperName = GameEngineString::ToUpper(_Paramter.AnimationName);

	if (Animation.end() != Animation.find(UpperName))
	{
		MsgAssert("�̹� �����ϴ� �̸��� �ִϸ��̼� �Դϴ�." + UpperName);
	}


	FrameAnimation& NewAnimation = Animation[UpperName];

	NewAnimation.Image = Image;

	if (_Paramter.FilterName != "")
	{
		NewAnimation.FilterImage = GameEngineResources::GetInst().TextureFind(_Paramter.FilterName);

		if (nullptr == NewAnimation.FilterImage)
		{
			MsgAssert("�������� �ʴ� �̹����� �����̼� ���͸� ����Ҽ� �����ϴ�.");
		}
	}

	if (0 != _Paramter.FrameIndex.size())
	{
		NewAnimation.FrameIndex = _Paramter.FrameIndex;
	}
	else
	{
		for (int i = _Paramter.Start; i <= _Paramter.End; ++i)
		{
			NewAnimation.FrameIndex.push_back(i);
		}
	}

	// �� �����Ӻ� �ð��� ����Ѵ�.
	if (0 != _Paramter.FrameTime.size())
	{
		NewAnimation.FrameTime = _Paramter.FrameTime;
	}
	else
	{
		for (int i = 0; i < NewAnimation.FrameIndex.size(); ++i)
		{
			NewAnimation.FrameTime.push_back(_Paramter.InterTime);
		}
	}

	NewAnimation.Loop = _Paramter.Loop;
	NewAnimation.Parent = this;
}

void GameEngineRenderer::ChangeAnimation(const std::string_view& _AnimationName, bool _ForceChange /*= false*/)
{

	std::string UpperName = GameEngineString::ToUpper(_AnimationName);

	if (Animation.end() == Animation.find(UpperName))
	{
		MsgAssert("�������� �ʴ� �ִϸ��̼����� �ٲٷ��� �߽��ϴ�." + UpperName);
	}

	// ������ �ٲ��� �ʴ� ��Ȳ���� �ִϸ��̼��� ������
	if (false == _ForceChange && CurrentAnimation == &Animation[UpperName])
	{
		return;
	}

	CurrentAnimation = &Animation[UpperName];

	CurrentAnimation->CurrentIndex = 0;
	// 0.1
	CurrentAnimation->CurrentTime = CurrentAnimation->FrameTime[CurrentAnimation->CurrentIndex];
}