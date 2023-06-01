//
// Created by Daniel on 17.12.2022.
//
#include <iostream>
#include <string>

class Point;

class Segment;

class Vector;

class Ray;

class Line;

class Circle;

class Vector {
 public:
  Vector();

  Vector(int64_t absciss, int64_t ordinate);

  Vector(const Vector& vector) = default;

  Vector& operator=(const Vector& vector) = default;

  int64_t GetX() const;

  int64_t GetY() const;

  friend Vector& operator+=(Vector& vector1, const Vector& vector2);

  friend Vector& operator-=(Vector& vector1, const Vector& vector2);

  friend Vector& operator*=(Vector& vector, const int64_t& value);

  friend Vector& operator/=(Vector& vector, const int64_t& value);

 private:
  int64_t absciss_;
  int64_t ordinate_;
};
Vector operator+(const Vector& vector1, const Vector& vector2);

Vector operator-(const Vector& vector1, const Vector& vector2);

Vector operator*(const Vector& vector, const int64_t& value);

Vector operator/(const Vector& vector, const int64_t& value);

Vector operator*(const int64_t& value, const Vector& vector);

Vector operator/(const int64_t& value, const Vector& vector);

int64_t operator^(const Vector& vector1, const Vector& vector2);

int64_t operator*(const Vector& vector1, const Vector& vector2);

bool operator==(const Vector& vector1, const Vector& vector2);

Vector operator-(const Vector& vector);

class IShape {
 public:
  virtual void Move(const Vector& vector) = 0;

  virtual bool ContainsPoint(const Point& point) const = 0;

  virtual bool CrossSegment(const Segment& segment) const = 0;

  virtual IShape* Clone() const = 0;

  virtual ~IShape() = default;
};

class Point : public IShape {
 public:
  Point();

  Point(const int64_t& absciss, const int64_t& ordinate);

  Point(const Point& other);

  void Move(const Vector& vector) override;

  Point* Clone() const override;

  bool ContainsPoint(const Point& other) const override;

  bool CrossSegment(const Segment& other) const override;

  int64_t GetX() const;

  int64_t GetY() const;

  friend Vector operator-(const Point& point, const Point& vector);

  friend Point& operator+=(Point& point, const Point& vector);

  friend Point& operator+=(Point& point, const Vector& vector);

  friend Point operator+(const Point& point, const Vector& vector);

  Point& operator=(const Point& other);

  friend bool operator==(const Point& point1, const Point& point2);

 private:
  int64_t absciss_;
  int64_t ordinate_;
};

class Segment : public IShape {
 public:
  Segment(const Point& begin, const Point& end);

  void Move(const Vector& vector) override;

  Segment* Clone() const override;

  bool ContainsPoint(const Point& other) const override;

  bool CrossSegment(const Segment& other) const override;

  Point GetA() const;

  Point GetB() const;

 private:
  Point begin_;
  Point end_;
};

class Line : public IShape {
 public:
  Line(const Point& point1, const Point& point2);

  Line(const int64_t& first_coef, const int64_t& second_coef,
       const int64_t& third_coef);

  void Move(const Vector& vector) override;

  Line* Clone() const override;

  bool ContainsPoint(const Point& point) const override;

  bool CrossSegment(const Segment& segment) const override;

  int64_t PointValue(const Point& point) const;

  int64_t GetA() const;

  int64_t GetB() const;

  int64_t GetC() const;

  Vector GetNormalVector() const;

 private:
  int64_t first_coef_;
  int64_t second_coef_;
  int64_t third_coef_;
};

class Ray : public IShape {
 public:
  Ray();

  Ray(const Point& begin, const Point& end);

  Ray(const Point& other_beg, const Vector& other_dir);

  Ray(const Ray& other);

  Ray& operator=(const Ray& other);

  void Move(const Vector& shift) override;

  Ray* Clone() const override;

  bool ContainsPoint(const Point& point) const override;

  bool CrossSegment(const Segment& segment) const override;

  Point GetA() const;

  Vector GetVector() const;

 private:
  Point begin_;
  Vector direction_;
};

class Circle : public IShape {
 public:
  Circle(const Point& other_center, const int64_t& other_rad);

  Circle(const Circle& other);

  void Move(const Vector& other) override;

  Circle* Clone() const override;

  Point GetCentre() const;

  int64_t GetRadius() const;

  bool ContainsPoint(const Point& other) const override;

  bool CrossSegment(const Segment& segment) const override;

  bool OnlyContainsPoint(const Point& other) const;

 private:
  Point centre_;
  int64_t radius_;
};

#ifndef GEOMETRY_GEOMETRY_HPP
#define GEOMETRY_GEOMETRY_HPP

#endif  // GEOMETRY_GEOMETRY_HPP
