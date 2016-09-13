#include "Shape.h"

namespace Shape {

	Rectangle::Rectangle() :
		X(0), Y(0), Width(0), Height(0) {
	}

	Rectangle::Rectangle(Pixel x, Pixel y, Pixel width, Pixel height) :
		X(x), Y(y), Width(width), Height(height) {
	};

	/// <remarks>
	/// 使用例:
	/// <code>
	///	Rectangle Rect1;
	/// Rect1.X = 100; Rect1.Y = 100;
	/// Rect1.Width = 100; Rect1.Height = 100;
	///
	///	Rectangle Rect2;
	/// Rect2.X = 150; Rect2.Y = 200;
	/// Rect2.Width = 100; Rect2.Height = 100;
	///
	/// if (Rect1.IntersectWith(Rect2)) {
	///		printf("Rect1 intersects Rect2!\n");
	/// } else {
	///		printf("Rect1 does not intersect Rect2.\n");
	/// }
	///
	/// // 出力
	/// // Rect1 intersects Rect2!
	/// </code>
	/// </remarks>
	bool Rectangle::IntersectWith(Rectangle& rect) const {
		bool VIntersect, HIntersect;

		if (X < rect.X)
			HIntersect = Right() >= rect.X;
		else
			HIntersect = rect.Right() >= X;

		if (Y < rect.Y)
			VIntersect = Bottom() >= rect.Y;
		else
			VIntersect = rect.Bottom() >= Y;

		return VIntersect && HIntersect;
	}

	/// <remarks>
	/// 使用例:
	/// <code>
	///	Rectangle Rect1;
	/// Rect1.X = 100; Rect1.Y = 100;
	/// Rect1.Width = 100; Rect1.Height = 100;
	///
	/// if (Rect1.IntersectWith( { 150, 200, 100, 100 } )) {
	///		printf("Rect1 intersects!\n");
	/// } else {
	///		printf("Rect1 does not intersect.\n");
	/// }
	///
	/// // 出力
	/// // Rect1 intersects!
	/// </code>
	/// </remarks>
	bool Rectangle::IntersectWith(Rectangle&& rect) const {
		bool VIntersect, HIntersect;

		if (X < rect.X)
			HIntersect = Right() > rect.X;
		else
			HIntersect = rect.Right() > X;

		if (Y < rect.Y)
			VIntersect = Bottom() > rect.Y;
		else
			VIntersect = rect.Bottom() > Y;

		return VIntersect && HIntersect;
	}

	/// <remarks>
	/// 使用例:
	/// <code>
	///	Rectangle Rect1;
	/// Rect1.X = 100; Rect1.Y = 100;
	/// Rect1.Width = 100; Rect1.Height = 100;
	///
	///	Rectangle Rect2;
	/// Rect2.X = 150; Rect2.Y = 150;
	/// Rect2.Width = 100; Rect2.Height = 100;
	///
	/// Rect1.Intersects(Rect2));
	/// printf("Rect1: x = %.0f, y = %.0f, width = %.0f, height = %.0f\n", Rect1.X, Rect1.Y, Rect1.Width, Rect1.Height);
	///
	/// // 出力
	/// // Rect1: x = 150, y = 150, width = 50, height = 50
	/// </code>
	/// </remarks>
	void Rectangle::Intersects(Rectangle &val) {
		Pixel Left, Right, Top, Bottom;

		if (X <= val.X) {
			Left = val.X; Right = X + Width;
		}
		else {
			Left = X; Right = val.X + val.Width;
		}

		if (Y <= val.Y) {
			Top = val.Y; Bottom = Y + Height;
		}
		else {
			Top = Y; Bottom = val.Y + val.Height;
		}

		if (Left < Right && Top < Bottom) {
			X = Left; Y = Top;
			Width = Right - Left; Height = Bottom - Top;
		}
		else {
			X = Y = Width = Height = 0;
		}
	}

	/// <remarks>
	/// 使用例:
	/// <code>
	///	Rectangle Rect1;
	/// Rect1.X = 100; Rect1.Y = 100;
	/// Rect1.Width = 100; Rect1.Height = 100;
	///
	/// Rect1.Intersects( { 150, 150, 100, 100 } ));
	/// printf("Rect1: x = %.0f, y = %.0f, width = %.0f, height = %.0f\n", Rect1.X, Rect1.Y, Rect1.Width, Rect1.Height);
	///
	/// // 出力
	/// // Rect1: x = 150, y = 150, width = 50, height = 50
	/// </code>
	/// </remarks>
	void Rectangle::Intersects(Rectangle &&val) {
		Pixel Left, Right, Top, Bottom;

		if (X <= val.X) {
			Left = val.X; Right = X + Width;
		} else {
			Left = X; Right = val.X + val.Width;
		}

		if (Y <= val.Y) {
			Top = val.Y; Bottom = Y + Height;
		} else {
			Top = Y; Bottom = val.Y + val.Height;
		}

		if (Left < Right && Top < Bottom) {
			X = Left; Y = Top;
			Width = Right - Left; Height = Bottom - Top;
		} else {
			X = Y = Width = Height = 0;
		}
	}

	bool RectangleContains(Rectangle rect, int x, int y) {
		return rect.Contains(x, y);
	}

	bool RectangleIntersectWith(Rectangle rect1, Rectangle rect2) {
		return rect1.IntersectWith(rect2);
	}

	bool IsRectanglesIntersect(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
		bool VIntersect, HIntersect;

		HIntersect = (x1 < x2) ? (x1 + width1 >= x2) : (x2 + width2 >= x1);
		VIntersect = (y1 < y2) ? (y1 + height1 >= y2) : (y2 + height2 >= y1);

		return VIntersect && HIntersect;
	}


	Rectangle RectangleIntersects(Rectangle rect1, Rectangle rect2) {
		Rectangle rect = rect1;
		rect.Intersects(rect2);
		return rect;
	}
}