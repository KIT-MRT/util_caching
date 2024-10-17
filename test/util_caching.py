import os
import unittest
import time

from util_caching_py import number_based, time_based


class CacheTest(unittest.TestCase):
    def setUp(self):
        self.key1 = 1.0
        self.key2 = 1.2
        self.key3 = 1.6
        self.time1 = time.time()
        self.time2 = self.time1 + 0.01  # 10 milliseconds later
        self.time3 = self.time1 + 1.1  # 1100 milliseconds later
        self.time4 = self.time1 + 2.1  # 2100 milliseconds later
        self.cache_by_number = number_based.Cache()
        self.cache_by_time = time_based.Cache()
        self.approximate_number_policy = number_based.ApproximateNumber(0.5)
        self.approximate_time_policy = time_based.ApproximateTime(100)
        self.approximate_time_policy_2 = time_based.ApproximateTime(1000)

    def test_with_number_key(self):
        self.assertIsNone(self.cache_by_number.cached(self.key1))
        self.cache_by_number.cache(self.key1, 1.0)

        # exact match
        self.assertTrue(self.cache_by_number.cached(self.key1))
        self.assertEqual(self.cache_by_number.cached(self.key1), 1.0)

        # approximate match
        self.assertTrue(
            self.cache_by_number.cached(self.key2, self.approximate_number_policy)
        )
        self.assertEqual(
            self.cache_by_number.cached(self.key2, self.approximate_number_policy),
            1.0,
        )

        # over threshold
        self.assertIsNone(
            self.cache_by_number.cached(self.key3, self.approximate_number_policy)
        )

    def test_with_time_key(self):
        self.assertIsNone(self.cache_by_time.cached(self.time1))
        self.cache_by_time.cache(self.time1, 1.0)

        # exact match
        self.assertTrue(self.cache_by_time.cached(self.time1))
        self.assertEqual(self.cache_by_time.cached(self.time1), 1.0)

        # approximate match with milliseconds
        self.assertTrue(
            self.cache_by_time.cached(self.time2, self.approximate_time_policy)
        )
        self.assertEqual(
            self.cache_by_time.cached(self.time2, self.approximate_time_policy), 1.0
        )

        # approximate match with seconds
        self.assertTrue(
            self.cache_by_time.cached(self.time2, self.approximate_time_policy_2)
        )
        self.assertEqual(
            self.cache_by_time.cached(self.time2, self.approximate_time_policy_2), 1.0
        )

        # over threshold
        self.assertIsNone(
            self.cache_by_time.cached(self.time3, self.approximate_time_policy)
        )
        # expect 2s after rounding to integer which is over threshold
        self.assertIsNone(
            self.cache_by_time.cached(self.time4, self.approximate_time_policy_2)
        )


if __name__ == "__main__":
    header = "Running " + os.path.basename(__file__)

    print("=" * len(header))
    print(header)
    print("=" * len(header) + "\n")
    unittest.main(exit=False)
    print("=" * len(header) + "\n")
