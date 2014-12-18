import unittest
from gtsam import *
import numpy_eigen as npe

#https://docs.python.org/2/library/unittest.html
class TestPoint2(unittest.TestCase):

    def setUp(self):
        self.point = Point2()

    def test_get_methods(self):
        x = self.point.x()
        y = self.point.y()
        self.assertEqual(x, 0.0)
        self.assertEqual(y, 0.0)

    def test_overloaded_constructor(self):
        p = Point2(1.0, 1.0)
        x = p.x()
        y = p.y()
        self.assertEqual(x, 1.0)
        self.assertEqual(y, 1.0)

    def test_group_methods(self):
        p = Point2(2.0, 5.0)
        actual = self.point.compose(p)
        x = actual.x()
        y = actual.y()
        self.assertEqual(x, 2.0)
        self.assertEqual(y, 5.0)

        actual = p.inverse()
        x = actual.x()
        y = actual.y()
        self.assertEqual(x, -2.0)
        self.assertEqual(y, -5.0)

        p2 = Point2(1.0, 4.0)
        actual = p2.between(p)
        x = actual.x()
        y = actual.y()
        self.assertEqual(x, 1.0)
        self.assertEqual(y, 1.0)

    def test_vector(self):
        self.point.vector()



if __name__ == '__main__':
    unittest.main()
