import sys
import unittest

import pytest
import numpy as np

from ray.util.client.ray_client_helpers import ray_start_client_server
from ray.util.debug import _logged


@pytest.fixture(autouse=True)
def reset_debug_logs():
    """Resets internal state of ray.util.debug so that warning can be tested
    more than once in the same process"""
    _logged.clear()


class LoggerSuite(unittest.TestCase):
    """Test client warnings are raised when many tasks are scheduled"""

    def testOutboundMessageSizeWarning(self):
        with self.assertWarns(UserWarning) as cm, ray_start_client_server() as ray:
            large_argument = np.random.rand(100, 100, 100)

            @ray.remote
            def f(some_arg):
                return some_arg[0][0][0]

            for _ in range(50):
                f.remote(large_argument)
        assert (
            "More than 10MB of messages have been created to "
            "schedule tasks on the server." in cm.warning.args[0]
        )


if __name__ == "__main__":
    sys.exit(pytest.main(["-sv", __file__]))
