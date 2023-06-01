#include "geometry.hpp"
//
// Updated by Daniel on 22.12.2022.
//
Vector::Vector() : absciss_(0), ordinate_(0) {}

Vector::Vector(int64_t absciss, int64_t ordinate)
    : absciss_(absciss), ordinate_(ordinate) {}

Vector operator-(const Vector& vector) {
  return {vector.GetX(), vector.GetY()};
}

Vector operator+(const Vector& vector1, const Vector& vector2) {
  return {vector1.GetX() + vector2.GetX(), vector1.GetY() + vector2.GetY()};
}

Vector operator-(const Vector& vector1, const Vector& vector2) {
  return {vector1.GetX() - vector2.GetX(), vector1.GetY() - vector2.GetY()};
}

Vector operator*(const Vector& vector, const int64_t& value) {
  return {vector.GetX() * value, vector.GetY() * value};
}

Vector operator/(const Vector& vector, const int64_t& value) {
  return {vector.GetX() / value, vector.GetY() / value};
}

Vector operator*(const int64_t& value, const Vector& vector) {
  return {vector.GetX() * value, vector.GetY() * value};
}

Vector operator/(const int64_t& value, const Vector& vector) {
  return {vector.GetX() / value, vector.GetY() / value};
}

Vector& operator+=(Vector& vector1, const Vector& vector2) {
  vector1.absciss_ += vector2.absciss_;
  vector1.ordinate_ += vector2.ordinate_;
  return vector1;
}

Vector& operator-=(Vector& vector1, const Vector& vector2) {
  vector1.absciss_ -= vector2.absciss_;
  vector1.ordinate_ -= vector2.ordinate_;
  return vector1;
}

Vector& operator*=(Vector& vector1, const int64_t& value) {
  vector1.absciss_ *= value;
  vector1.ordinate_ *= value;
  return vector1;
}

Vector& operator/=(Vector& vector, const int64_t& value) {
  vector.absciss_ /= value;
  vector.ordinate_ /= value;
  return vector;
}

bool operator==(const Vector& vector1, const Vector& vector2) {
  return (vector1.GetX() == vector2.GetX() && vector1.GetY() == vector2.GetY());
}

int64_t Vector::GetX() const { return this->absciss_; }

int64_t Vector::GetY() const { return this->ordinate_; }

int64_t operator^(const Vector& vector1, const Vector& vector2) {
  return vector1.GetX() * vector2.GetY() - vector1.GetY() * vector2.GetX();
}

int64_t operator*(const Vector& vector1, const Vector& vector2) {
  return vector1.GetX() * vector2.GetX() + vector1.GetY() * vector2.GetY();
}

Point::Point() : absciss_(0), ordinate_(0) {}

Point::Point(const int64_t& absciss, const int64_t& ordinate)
    : absciss_(absciss), ordinate_(ordinate) {}

Point::Point(const Point& other)
    : absciss_(other.absciss_), ordinate_(other.ordinate_) {}

void Point::Move(const Vector& vector) {
  this->absciss_ += vector.GetX();
  this->ordinate_ += vector.GetY();
}

Point* Point::Clone() const { return new Point(absciss_, ordinate_); }

bool Point::ContainsPoint(const Point& other) const {
  return (this->absciss_ == other.absciss_ &&
          this->ordinate_ == other.ordinate_);
}

bool Point::CrossSegment(const Segment& other) const {
  Vector temp_vec1 = {other.GetB() - *this};
  Vector temp_vec2 = {*this - other.GetA()};
  return ((temp_vec1 ^ temp_vec2) == 0) && (temp_vec1 * temp_vec2) >= 0;
}

Point& Point::operator=(const Point& other) {
  this->absciss_ = other.absciss_;
  this->absciss_ = other.ordinate_;
  return *this;
}

Vector operator-(const Point& point1, const Point& vector) {
  return {point1.absciss_ - vector.absciss_,
          point1.ordinate_ - vector.ordinate_};
}

Point& operator+=(Point& point1, const Point& vector) {
  point1.absciss_ += vector.absciss_;
  point1.ordinate_ += vector.ordinate_;
  return point1;
}

Point& operator+=(Point& point, const Vector& vector) {
  point.absciss_ += vector.GetX();
  point.ordinate_ += vector.GetY();
  return point;
}

int64_t Point::GetX() const { return this->absciss_; }

int64_t Point::GetY() const { return this->ordinate_; }

Point operator+(const Point& point, const Vector& vector) {
  Point res(point);
  return (res += vector);
}

bool operator==(const Point& point1, const Point& point2) {
  return (point1.GetX() == point2.GetX() && point1.GetY() == point2.GetY());
}

Segment::Segment(const Point& begin, const Point& end)
    : begin_(begin), end_(end) {}

Point Segment::GetA() const { return this->begin_; }

Point Segment::GetB() const { return this->end_; }

void Segment::Move(const Vector& vector) {
  Point temp_p = {vector.GetX(), vector.GetY()};
  this->begin_ += temp_p;
  this->end_ += temp_p;
}

Segment* Segment::Clone() const {
  return new Segment(this->begin_, this->end_);
}

bool Segment::ContainsPoint(const Point& other) const {
  return other.CrossSegment(*this);
}

bool Segment::CrossSegment(const Segment& other) const {
  Line our_line(this->begin_, this->end_);
  Line other_line(other.begin_, other.end_);

  if (our_line.CrossSegment(other) && other_line.CrossSegment(*this)) {
    bool check1 = std::max(this->begin_.GetX(), this->end_.GetX()) >=
                  std::min(other.begin_.GetX(), other.end_.GetX());
    bool check2 = std::max(this->begin_.GetY(), this->end_.GetY()) >=
                  std::min(other.begin_.GetY(), other.end_.GetY());
    return check1 && check2;
  }

  return false;
}

Line::Line(const Point& point1, const Point& point2) {
  if ((point1.GetX() - point2.GetX()) == 0) {
    this->first_coef_ = 1;
    this->second_coef_ = 0;
    this->third_coef_ = -point1.GetX();

  } else if ((point1.GetY() - point2.GetY()) == 0) {
    this->first_coef_ = 0;
    this->second_coef_ = 1;
    this->third_coef_ = -point1.GetY();
  } else {
    Vector guide_vector = point2 - point1;
    this->first_coef_ = -guide_vector.GetY();
    this->second_coef_ = guide_vector.GetX();
    this->third_coef_ =
        -point1.GetX() * this->first_coef_ - point1.GetY() * this->second_coef_;
  }
}

Line::Line(const int64_t& first_coef, const int64_t& second_coef,
           const int64_t& third_coef)
    : first_coef_(first_coef),
      second_coef_(second_coef),
      third_coef_(third_coef) {}

void Line::Move(const Vector& vector) {
  if (this->first_coef_ == 0) {
    this->third_coef_ -= vector.GetY();

  } else if (this->second_coef_ == 0) {
    this->third_coef_ -= vector.GetX();

  } else {
    this->third_coef_ = this->third_coef_ - this->first_coef_ * vector.GetX() -
                        this->second_coef_ * vector.GetY();
  }
}

bool Line::ContainsPoint(const Point& point) const {
  return (first_coef_ * point.GetX() + second_coef_ * point.GetY() +
              third_coef_ ==
          0);
}

bool Line::CrossSegment(const Segment& segment) const {
  int64_t val_begin = this->first_coef_ * segment.GetA().GetX() +
                      this->second_coef_ * segment.GetA().GetY() +
                      this->third_coef_;
  int64_t val_end = this->first_coef_ * segment.GetB().GetX() +
                    this->second_coef_ * segment.GetB().GetY() +
                    this->third_coef_;

  return (val_begin * val_end <= 0);
}

Line* Line::Clone() const {
  return new Line(first_coef_, second_coef_, third_coef_);
}

int64_t Line::PointValue(const Point& point) const {
  return first_coef_ * point.GetX() + second_coef_ * point.GetY() + third_coef_;
}

int64_t Line::GetA() const { return first_coef_; }

int64_t Line::GetB() const { return second_coef_; }

int64_t Line::GetC() const { return third_coef_; }

Vector Line::GetNormalVector() const { return {first_coef_, second_coef_}; }

Ray::Ray() : begin_({0, 0}), direction_({0, 0}) {}

Ray::Ray(const Point& begin, const Point& end)
    : begin_(begin), direction_(end - begin) {}

Ray::Ray(const Point& other_beg, const Vector& other_dir)
    : begin_(other_beg), direction_(other_dir) {}

Ray::Ray(const Ray& other)
    : begin_(other.begin_), direction_(other.direction_) {}

void Ray::Move(const Vector& shift) { this->begin_ += shift; }

Ray* Ray::Clone() const { return new Ray(begin_, direction_); }

bool Ray::ContainsPoint(const Point& point) const {
  Vector temp = point - begin_;
  return (((temp ^ direction_) == 0) && (temp * direction_) >= 0);
}

bool Ray::CrossSegment(const Segment& segment) const {
  if (segment.ContainsPoint(this->begin_)) {
    return true;
  }

  Line line_vec(this->begin_, this->begin_ + this->direction_);

  if (line_vec.CrossSegment(segment)) {
    Vector vec_seg = segment.GetB() - segment.GetA();
    Vector temp_vec = begin_ - segment.GetA();

    int64_t cross_product1 = (vec_seg ^ temp_vec);
    int64_t cross_product2 = (vec_seg ^ direction_);

    return cross_product1 * cross_product2 <= 0;
  }

  return false;
}

Point Ray::GetA() const { return begin_; }

Vector Ray::GetVector() const { return direction_; }

Ray& Ray::operator=(const Ray& other) {
  this->begin_ = other.GetA();
  this->direction_ = other.GetVector();
  return *this;
}

Circle::Circle(const Point& other_center, const int64_t& other_rad)
    : centre_(other_center), radius_(other_rad) {}

Circle::Circle(const Circle& other)
    : centre_(other.centre_), radius_(other.radius_) {}

Point Circle::GetCentre() const { return centre_; }

int64_t Circle::GetRadius() const { return radius_; }

void Circle::Move(const Vector& other) { centre_ += other; }

Circle* Circle::Clone() const { return new Circle(centre_, radius_); }

bool Circle::ContainsPoint(const Point& other) const {
  int64_t first = (other.GetX() - this->centre_.GetX()) *
                  (other.GetX() - this->centre_.GetX());
  int64_t second = (other.GetY() - this->centre_.GetY()) *
                   (other.GetY() - this->centre_.GetY());
  int64_t deg_rad = this->radius_ * this->radius_;

  return (first + second <= deg_rad);
}

bool Circle::OnlyContainsPoint(const Point& other) const {
  int64_t first = (other.GetX() - this->centre_.GetX()) *
                  (other.GetX() - this->centre_.GetX());
  int64_t second = (other.GetY() - this->centre_.GetY()) *
                   (other.GetY() - this->centre_.GetY());
  int64_t deg_rad = radius_ * radius_;

  return (first + second < deg_rad);
}

bool Circle::CrossSegment(const Segment& segment) const {
  if (ContainsPoint(segment.GetA()) && !ContainsPoint(segment.GetB()) ||
      !ContainsPoint(segment.GetA()) && ContainsPoint(segment.GetB())) {
    return true;
  }

  if (OnlyContainsPoint(segment.GetA()) && OnlyContainsPoint(segment.GetB())) {
    return false;
  }

  Line other_line(segment.GetA(), segment.GetB());
  int64_t val = other_line.PointValue(this->centre_);
  int64_t len = val * val;
  if (len > radius_ * radius_ *
                (other_line.GetA() * other_line.GetA() +
                 other_line.GetB() * other_line.GetB())) {
    return false;
  }
  Vector other_normal = other_line.GetNormalVector();
  Line normal_line(centre_, centre_ + other_normal);

  return normal_line.CrossSegment(segment);
}
