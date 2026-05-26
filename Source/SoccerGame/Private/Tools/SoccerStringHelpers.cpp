// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerStringHelpers.h"

FString USoccerStringHelpers::Sanitize(const FString& Input)
{
	FString Sanitized = Input;
	Sanitized.TrimStartAndEndInline();
	Sanitized.ReplaceInline(TEXT("\n"), TEXT(" "));
	Sanitized.ReplaceInline(TEXT("\t"), TEXT(" "));
	Sanitized.ReplaceInline(TEXT("  "), TEXT(" "));
	return Sanitized;
}

FString USoccerStringHelpers::ToTitleCase(const FString& Input)
{
	FString Result = Input;
	Result.TrimStartAndEndInline();

	if (!Result.IsEmpty())
	{
		Result[0] = FChar::ToUpper(Result[0]);
		for (int32 Index = 1; Index < Result.Len(); ++Index)
		{
			Result[Index] = FChar::ToLower(Result[Index]);
		}
	}

	return Result;
}

FString USoccerStringHelpers::FormatPlayerLabel(int32 TeamId, int32 PlayerNumber, const FString& Position)
{
	return FString::Printf(TEXT("Team %d - #%d - %s"), TeamId, PlayerNumber, *Position);
}

bool USoccerStringHelpers::IsNullOrEmpty(const FString& Input)
{
	return Input.IsEmpty() || Input.TrimStartAndEnd().IsEmpty();
}
