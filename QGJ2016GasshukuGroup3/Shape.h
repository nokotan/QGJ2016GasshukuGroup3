#pragma once

/// @file Shape.h
/// Shape.h --------------
/// 幾何図形を定義します。
/// 最終更新:
///			2016/08/13
/// ----------------------

namespace Shape {

	/// <summary>座標の値を保持する型</summary>
	typedef int Pixel;

	/// <summary>辺が x, y 軸に平行な長方形を定義します。</summary>
	struct Rectangle {
		Pixel X, Y, Width, Height;

		/// <summary>空の長方形を作成します。</summary>
		Rectangle();
		/// <summary>指定した x, y座標, 幅, 高さから長方形を作成します。</summary>
		/// <param name="x">新しく作成する長方形の x 座標</param>
		/// <param name="y">新しく作成する長方形の y 座標</param>
		/// <param name="width">新しく作成する長方形の幅</param>
		/// <param name="height">新しく作成する長方形の高さ</param>
		Rectangle(Pixel x, Pixel y, Pixel width, Pixel height);

		/// <summary>右端の座標を取得します。</summary>
		/// <returns>右端の座標</returns>
		inline Pixel Right() const {
			return (X + Width - 1);
		}

		/// <summary>下端の座標を取得します。</summary>
		/// <returns>下端の座標</returns>
		inline Pixel Bottom() const {
			return (Y + Height - 1);
		}

		/// <summary>指定した座標が含まれているかどうか調べます。</summary>
		/// <param name="x">含まれているかどうか確かめる点の x 座標の値</param>
		/// <param name="y">含まれているかどうか確かめる点の y 座標の値</param>
		/// <returns>含まれていれば true, そうでなければ false</returns>
		/// <remarks>
		/// 使用例:
		/// <code>
		///	Rectangle Rect1;
		/// Rect1.X = 100; Rect1.Y = 100;
		/// Rect1.Width = 100; Rect1.Height = 100;
		///
		///	int MouseX, MouseY;
		/// GetMousePoint(&amp;MouseX, &amp;MouseY);
		///
		/// if (Rect1.Contains(MouseX, MouseY)) {
		///		// マウスカーソルが長方形上にあればこっち
		/// } else {
		///		// マウスカーソルが長方形上になければこっち
		/// }
		/// </code>
		/// </remarks>
		inline bool Contains(int x, int y) const {
			return (X <= x && x <= Right() && Y <= y && y <= Bottom());
		}

		/// <summary>指定した長方形と重なっているかどうか調べます。</summary>
		/// <param name="rect">重なっているかどうか確かめる長方形</param>
		/// <returns>重なっていれば true, そうでなければ false</returns>
		bool IntersectWith(Rectangle& rect) const;

		/// <summary>指定した長方形と重なっているかどうか調べます。</summary>
		/// <param name="rect">重なっているかどうか確かめる長方形</param>
		/// <returns>重なっていれば true, そうでなければ false</returns>
		bool IntersectWith(Rectangle&& rect) const;

		/// <summary>この長方形を指定した長方形との交差部分に変換します</summary>
		/// <param name="rect">交差部分を取る長方形</param>
		void Intersects(Rectangle& rect);

		/// <summary>この長方形を指定した長方形との交差部分に変換します</summary>
		/// <param name="rect">交差部分を取る長方形</param>
		void Intersects(Rectangle&& rect);
	};

	typedef Rectangle Rectangle_t;

	/// <summary>指定した長方形にある座標が含まれているかどうか調べます。</summary>
	/// <param name="rect">ある座標が含まれているかどうか調べる対象の長方形</param>
	/// <param name="x">含まれているかどうか確かめる点の x 座標の値</param>
	/// <param name="y">含まれているかどうか確かめる点の y 座標の値</param>
	/// <returns>含まれていれば true, そうでなければ false</returns>
	/// <remarks>
	/// 使用例:
	/// <code>
	///	Rectangle Rect1;
	/// Rect1.X = 100; Rect1.Y = 100;
	/// Rect1.Width = 100; Rect1.Height = 100;
	///
	///	int MouseX, MouseY;
	/// GetMousePoint(&amp;MouseX, &amp;MouseY);
	///
	/// if (RectangleContains(Rect1, MouseX, MouseY)) {
	///		// マウスカーソルが長方形上にあればこっち
	/// } else {
	///		// マウスカーソルが長方形上になければこっち
	/// }
	/// </code>
	/// </remarks>
	bool RectangleContains(Rectangle rect, int x, int y);

	/// <summary>２つの長方形が重なっているかどうか調べます。</summary>
	/// <param name="rect1">重なっているかどうか確かめる長方形</param>
	/// <param name="rect2">重なっているかどうか確かめる長方形</param>
	/// <returns>重なっていれば true, そうでなければ false</returns>
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
	/// if (RectangleIntersectWith(Rect1, Rect2)) {
	///		printf("Rect1 intersects Rect2!\n");
	/// } else {
	///		printf("Rect1 does not intersect Rect2.\n");
	/// }
	///
	/// // 出力
	/// // Rect1 intersects Rect2!
	/// </code>
	/// </remarks>
	bool RectangleIntersectWith(Rectangle rect1, Rectangle rect2);

	/// <summary>四角同士のあたり判定を行います。</summary>
	/// <returns>２つの四角が重なっていればtrue, そうでなければfalse</returns>
	/// <param name="x1">1個目の四角のx座標</param>
	/// <param name="y1">1個目の四角のy座標</param>
	/// <param name="width1">1個目の四角の幅</param>
	/// <param name="height1">1個目の四角の高さ</param>
	/// <param name="x2">2個目の四角のx座標</param>
	/// <param name="y2">2個目の四角のy座標</param>
	/// <param name="width2">2個目の四角の幅</param>
	/// <param name="height2">2個目の四角の高さ</param>
	/// <remarks>
	/// 使用例:
	/// <code>
	/// if (RectangleIntersectWith(100, 100, 100, 100, 150, 200, 100, 100)) {
	///		printf("Rect1 intersects Rect2!\n");
	/// } else {
	///		printf("Rect1 does not intersect Rect2.\n");
	/// }
	///
	/// // 出力
	/// // Rect1 intersects Rect2!
	/// </code>
	/// </remarks>
	bool RectangleIntersectWith(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);


	/// <summary>２つの長方形の交差部分を取得します。</summary>
	/// <param name="rect1">交差部分を取る対象の長方形</param>
	/// <param name="rect2">交差部分を取る対象の長方形</param>
	/// <returns>２つの長方形の交差部分を表す Recta</returns>
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
	/// Rectanlge Rect3 = RectangleIntersects(Rect1, Rect2);
	/// printf("Rect3: x = %.0f, y = %.0f, width = %.0f, height = %.0f\n", Rect3.X, Rect3.Y, Rect3.Width, Rect3.Height);
	///
	/// // 出力
	/// // Rect1: x = 150, y = 150, width = 50, height = 50
	/// </code>
	/// </remarks>
	Rectangle RectangleIntersects(Rectangle rect1, Rectangle rect2);
}

using namespace Shape;