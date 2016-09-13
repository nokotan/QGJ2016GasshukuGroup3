#pragma once

/// @file Shape.h
/// Shape.h --------------
/// �􉽐}�`���`���܂��B
/// �ŏI�X�V:
///			2016/08/13
/// ----------------------

namespace Shape {

	/// <summary>���W�̒l��ێ�����^</summary>
	typedef int Pixel;

	/// <summary>�ӂ� x, y ���ɕ��s�Ȓ����`���`���܂��B</summary>
	struct Rectangle {
		Pixel X, Y, Width, Height;

		/// <summary>��̒����`���쐬���܂��B</summary>
		Rectangle();
		/// <summary>�w�肵�� x, y���W, ��, �������璷���`���쐬���܂��B</summary>
		/// <param name="x">�V�����쐬���钷���`�� x ���W</param>
		/// <param name="y">�V�����쐬���钷���`�� y ���W</param>
		/// <param name="width">�V�����쐬���钷���`�̕�</param>
		/// <param name="height">�V�����쐬���钷���`�̍���</param>
		Rectangle(Pixel x, Pixel y, Pixel width, Pixel height);

		/// <summary>�E�[�̍��W���擾���܂��B</summary>
		/// <returns>�E�[�̍��W</returns>
		inline Pixel Right() const {
			return (X + Width - 1);
		}

		/// <summary>���[�̍��W���擾���܂��B</summary>
		/// <returns>���[�̍��W</returns>
		inline Pixel Bottom() const {
			return (Y + Height - 1);
		}

		/// <summary>�w�肵�����W���܂܂�Ă��邩�ǂ������ׂ܂��B</summary>
		/// <param name="x">�܂܂�Ă��邩�ǂ����m���߂�_�� x ���W�̒l</param>
		/// <param name="y">�܂܂�Ă��邩�ǂ����m���߂�_�� y ���W�̒l</param>
		/// <returns>�܂܂�Ă���� true, �����łȂ���� false</returns>
		/// <remarks>
		/// �g�p��:
		/// <code>
		///	Rectangle Rect1;
		/// Rect1.X = 100; Rect1.Y = 100;
		/// Rect1.Width = 100; Rect1.Height = 100;
		///
		///	int MouseX, MouseY;
		/// GetMousePoint(&amp;MouseX, &amp;MouseY);
		///
		/// if (Rect1.Contains(MouseX, MouseY)) {
		///		// �}�E�X�J�[�\���������`��ɂ���΂�����
		/// } else {
		///		// �}�E�X�J�[�\���������`��ɂȂ���΂�����
		/// }
		/// </code>
		/// </remarks>
		inline bool Contains(int x, int y) const {
			return (X <= x && x <= Right() && Y <= y && y <= Bottom());
		}

		/// <summary>�w�肵�������`�Əd�Ȃ��Ă��邩�ǂ������ׂ܂��B</summary>
		/// <param name="rect">�d�Ȃ��Ă��邩�ǂ����m���߂钷���`</param>
		/// <returns>�d�Ȃ��Ă���� true, �����łȂ���� false</returns>
		bool IntersectWith(Rectangle& rect) const;

		/// <summary>�w�肵�������`�Əd�Ȃ��Ă��邩�ǂ������ׂ܂��B</summary>
		/// <param name="rect">�d�Ȃ��Ă��邩�ǂ����m���߂钷���`</param>
		/// <returns>�d�Ȃ��Ă���� true, �����łȂ���� false</returns>
		bool IntersectWith(Rectangle&& rect) const;

		/// <summary>���̒����`���w�肵�������`�Ƃ̌��������ɕϊ����܂�</summary>
		/// <param name="rect">������������钷���`</param>
		void Intersects(Rectangle& rect);

		/// <summary>���̒����`���w�肵�������`�Ƃ̌��������ɕϊ����܂�</summary>
		/// <param name="rect">������������钷���`</param>
		void Intersects(Rectangle&& rect);
	};

	typedef Rectangle Rectangle_t;

	/// <summary>�w�肵�������`�ɂ�����W���܂܂�Ă��邩�ǂ������ׂ܂��B</summary>
	/// <param name="rect">������W���܂܂�Ă��邩�ǂ������ׂ�Ώۂ̒����`</param>
	/// <param name="x">�܂܂�Ă��邩�ǂ����m���߂�_�� x ���W�̒l</param>
	/// <param name="y">�܂܂�Ă��邩�ǂ����m���߂�_�� y ���W�̒l</param>
	/// <returns>�܂܂�Ă���� true, �����łȂ���� false</returns>
	/// <remarks>
	/// �g�p��:
	/// <code>
	///	Rectangle Rect1;
	/// Rect1.X = 100; Rect1.Y = 100;
	/// Rect1.Width = 100; Rect1.Height = 100;
	///
	///	int MouseX, MouseY;
	/// GetMousePoint(&amp;MouseX, &amp;MouseY);
	///
	/// if (RectangleContains(Rect1, MouseX, MouseY)) {
	///		// �}�E�X�J�[�\���������`��ɂ���΂�����
	/// } else {
	///		// �}�E�X�J�[�\���������`��ɂȂ���΂�����
	/// }
	/// </code>
	/// </remarks>
	bool RectangleContains(Rectangle rect, int x, int y);

	/// <summary>�Q�̒����`���d�Ȃ��Ă��邩�ǂ������ׂ܂��B</summary>
	/// <param name="rect1">�d�Ȃ��Ă��邩�ǂ����m���߂钷���`</param>
	/// <param name="rect2">�d�Ȃ��Ă��邩�ǂ����m���߂钷���`</param>
	/// <returns>�d�Ȃ��Ă���� true, �����łȂ���� false</returns>
	/// <remarks>
	/// �g�p��:
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
	/// // �o��
	/// // Rect1 intersects Rect2!
	/// </code>
	/// </remarks>
	bool RectangleIntersectWith(Rectangle rect1, Rectangle rect2);

	/// <summary>�l�p���m�̂����蔻����s���܂��B</summary>
	/// <returns>�Q�̎l�p���d�Ȃ��Ă����true, �����łȂ����false</returns>
	/// <param name="x1">1�ڂ̎l�p��x���W</param>
	/// <param name="y1">1�ڂ̎l�p��y���W</param>
	/// <param name="width1">1�ڂ̎l�p�̕�</param>
	/// <param name="height1">1�ڂ̎l�p�̍���</param>
	/// <param name="x2">2�ڂ̎l�p��x���W</param>
	/// <param name="y2">2�ڂ̎l�p��y���W</param>
	/// <param name="width2">2�ڂ̎l�p�̕�</param>
	/// <param name="height2">2�ڂ̎l�p�̍���</param>
	/// <remarks>
	/// �g�p��:
	/// <code>
	/// if (RectangleIntersectWith(100, 100, 100, 100, 150, 200, 100, 100)) {
	///		printf("Rect1 intersects Rect2!\n");
	/// } else {
	///		printf("Rect1 does not intersect Rect2.\n");
	/// }
	///
	/// // �o��
	/// // Rect1 intersects Rect2!
	/// </code>
	/// </remarks>
	bool RectangleIntersectWith(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);


	/// <summary>�Q�̒����`�̌����������擾���܂��B</summary>
	/// <param name="rect1">�������������Ώۂ̒����`</param>
	/// <param name="rect2">�������������Ώۂ̒����`</param>
	/// <returns>�Q�̒����`�̌���������\�� Recta</returns>
	/// <remarks>
	/// �g�p��:
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
	/// // �o��
	/// // Rect1: x = 150, y = 150, width = 50, height = 50
	/// </code>
	/// </remarks>
	Rectangle RectangleIntersects(Rectangle rect1, Rectangle rect2);
}

using namespace Shape;