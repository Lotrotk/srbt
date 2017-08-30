#pragma once

namespace SRBT
{
namespace Parser
{
	enum Operators
	{
		kBraceLeft = 1,
		kBraceRight,
		kCurlyBraceLeft,
		kCurlyBraceRight,
		kSquareBraceLeft,
		kSquareBraceRight,
		kPoint,
		kComma,
		kColon,
		kSemicolon,
		kQuestion,
		kExclemation,
		kPlus,
		kMinus,
		kAsterisc,
		kForwardSlash,
		kBackwardSlash,
		kPercent,
		kAmpersand,
		kAt,
		kDollar,
		kHashtag,
		kHat,
		kIs,
		kOr,
		kLess,
		kMore,
		kCompareEquals,// ==
		kCompareDiffers,// !=
		kLessOrEqual,// <=
		kMoreOrEqual,// >=
	};

	enum Keywords
	{
		kBool = 1000,
		kInteger,
		kReal,
		kWord,
		kStruct,
	};
}
}
