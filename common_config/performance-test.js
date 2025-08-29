import { sleep } from 'k6';
import http from 'k6/http';
import { htmlReport } from "https://raw.githubusercontent.com/benc-uk/k6-reporter/main/dist/bundle.js";

/**
 * k6 Performance Test Script
 * 
 * This script performs a load test against a target URL using configurable parameters
 * passed in as environment variables. The test measures response times and produces
 * an HTML report.
 * 
 * Environment Variables (examples):
 * - LOAD_TEST_DURATION=2m
 * - LOAD_TEST_VIRTUAL_USERS=100
 * - LOAD_TEST_P95_THRESHOLD=5000
 * - LOAD_TEST_URL=https://example.com
 * - LOAD_TEST_USERNAME=myuser
 * - LOAD_TEST_PASSWORD=mypassword
 */

// k6 execution options
export const options = {
  // Total duration of the test (default: 1 minute)
  duration: __ENV.LOAD_TEST_DURATION || '1m',

  // Number of concurrent virtual users (default: 50)
  vus: Number(__ENV.LOAD_TEST_VIRTUAL_USERS) || 50,

  // Performance thresholds — test fails if 95% of requests exceed the limit
  thresholds: {
    http_req_duration: [
      // Example: if LOAD_TEST_P95_THRESHOLD=5000, fail if p95 > 5000ms
      `p(95)<${__ENV.LOAD_TEST_P95_THRESHOLD || 10000}`
    ],
  },
};

// Default test function executed by each VU (virtual user)
export default function () {
  // Target system URL and Basic Auth credentials from environment
  const url = __ENV.LOAD_TEST_URL;
  const username = __ENV.LOAD_TEST_USERNAME;
  const password = __ENV.LOAD_TEST_PASSWORD;

  // Make an HTTP GET request with Basic Authentication
  const res = http.get(url, {
    auth: `${username}:${password}`,
  });

  // Wait for 3 seconds before the next request (simulate user think-time)
  sleep(3);
}

// Generate HTML summary report at the end of the test
export function handleSummary(data) {
  return {
    "summary.html": htmlReport(data),
  };
}