import unittest
import source.util as util

class TestUtil(unittest.TestCase):
    
    def test_add(self):
        self.assertEqual(util.add(0, 1), 1)
        self.assertEqual(util.add(0, 10), 10)
        self.assertEqual(util.add(0, 255), 255)
        self.assertEqual(util.add(0, 256), 0)
        self.assertEqual(util.add(255, 1), 0)

    def test_subtract(self):
        self.assertEqual(util.subtract(10, 1), 9)
        self.assertEqual(util.subtract(10, 5), 5)
        self.assertEqual(util.subtract(10, 10), 0)
        self.assertEqual(util.subtract(0, 1), 255)
        self.assertEqual(util.subtract(2, 3), 255)
        self.assertEqual(util.subtract(2, 8), 250)
