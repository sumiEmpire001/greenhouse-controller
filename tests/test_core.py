import unittest
from greenhouse_controller import State, Settings, decide

class ControllerTests(unittest.TestCase):
    def test_hysteresis(self):
        state = decide(31, 20)
        self.assertEqual(state, State(True, True))
        state = decide(28, 40, state)
        self.assertEqual(state, State(True, True))
        self.assertEqual(decide(27, 45, state), State(False, False))
    def test_boundaries(self):
        self.assertEqual(decide(30, 30), State(True, True))
        self.assertEqual(decide(27, 45, State(True, True)), State(False, False))
    def test_invalid(self):
        with self.assertRaises(ValueError):
            Settings(temperature_on=25, temperature_off=26)
        with self.assertRaises(ValueError):
            decide(float("nan"), 30)
        with self.assertRaises(ValueError):
            decide(25, 101)
