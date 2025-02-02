#pragma once

#include <jwt-cpp/jwt.h>
#include <chrono>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace vpn_manager {

/// A helper class that provides common JWT operations:
/// - Creating (encoding) a token with custom payload claims and expiration.
/// - Decoding a token (without verification).
/// - Verifying a token’s signature and claims.
/// - Refreshing a token (copying its claims with a new expiration).
/// - Checking if a token is expired.
class JWTHelper {
 public:
  /// Constructs a JWTHelper.
  /// @param secret The secret key used for signing/verifying tokens.
  /// @param issuer The expected issuer claim (defaults to "my_issuer").
  /// @param default_expiration The default expiration duration (defaults to
  /// 3600 seconds).
  JWTHelper(
      const std::string& secret, const std::string& issuer = "my_issuer",
      std::chrono::seconds default_expiration = std::chrono::seconds{3600})
      : secret_(secret),
        issuer_(issuer),
        default_expiration_(default_expiration) {}

  /// Creates a JWT token with the provided payload claims and the default
  /// expiration.
  /// @param payload A map of claim names to their string values.
  /// @return A signed JWT token as a string.
  std::string CreateToken(
      const std::unordered_map<std::string, std::string>& payload) const {
    return CreateTokenWithExpiration(payload, default_expiration_);
  }

  /// Creates a JWT token with the provided payload claims and a specified
  /// expiration.
  /// @param payload A map of claim names to their string values.
  /// @param expiration The desired expiration duration.
  /// @return A signed JWT token as a string.
  std::string CreateTokenWithExpiration(
      const std::unordered_map<std::string, std::string>& payload,
      std::chrono::seconds expiration) const {
    auto now = std::chrono::system_clock::now();
    // Build the token, setting standard claims.
    auto token_builder =
        jwt::create().set_issuer(issuer_).set_issued_at(now).set_expires_at(
            now + expiration);

    // Add custom payload claims.
    for (const auto& kv : payload) {
      token_builder.set_payload_claim(kv.first, jwt::claim(kv.second));
    }

    // Sign the token with HS256 algorithm.
    return token_builder.sign(jwt::algorithm::hs256{secret_});
  }

  /// Decodes a JWT token without verifying its signature or claims.
  /// @param token The JWT token string.
  /// @return A jwt::decoded_jwt object representing the decoded token.
  jwt::decoded_jwt<jwt::traits::kazuho_picojson> DecodeToken(
      const std::string& token) const {
    return jwt::decode(token);
  }

  /// Verifies a JWT token's signature and issuer claim.
  /// Throws an exception if verification fails.
  /// @param token The JWT token string.
  /// @return A jwt::decoded_jwt object representing the verified token.
  jwt::decoded_jwt<jwt::traits::kazuho_picojson> VerifyToken(
      const std::string& token) const {
    auto decoded = jwt::decode(token);
    jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{secret_})
        .with_issuer(issuer_)
        .verify(decoded);
    return decoded;
  }

  /// Refreshes a valid JWT token by creating a new token with the same payload
  /// claims (except for reserved ones) and a new expiration time.
  /// @param token The original JWT token string.
  /// @param new_expiration The new expiration duration.
  /// @return A new signed JWT token as a string.
  std::string RefreshToken(const std::string& token,
                           std::chrono::seconds new_expiration) const {
    // First, verify the original token.
    auto decoded = VerifyToken(token);

    auto now = std::chrono::system_clock::now();
    auto token_builder =
        jwt::create().set_issuer(issuer_).set_issued_at(now).set_expires_at(
            now + new_expiration);

    // Use the payload from the decoded token (which is a JSON string).
    auto payload_str = decoded.get_payload();
    picojson::value payload_val;
    std::string err = picojson::parse(payload_val, payload_str);
    if (!err.empty() || !payload_val.is<picojson::object>()) {
      throw std::runtime_error("Invalid payload in token");
    }
    auto claims = payload_val.get<picojson::object>();

    // Copy all payload claims except for the reserved ones.
    for (const auto& kv : claims) {
      const auto& key = kv.first;
      if (key == "iss" || key == "iat" || key == "exp") {
        continue;  // Skip reserved claims.
      }
      token_builder.set_payload_claim(key, jwt::claim(kv.second));
    }
    return token_builder.sign(jwt::algorithm::hs256{secret_});
  }

  /// Checks if the token is expired.
  /// @param token The JWT token string.
  /// @return True if the token is expired; otherwise, false.
  bool IsTokenExpired(const std::string& token) const {
    auto decoded = jwt::decode(token);
    try {
      auto exp_claim = decoded.get_payload_claim("exp");
      auto exp_time =
          exp_claim.as_date();  // as_date() returns a
                                // std::chrono::system_clock::time_point
      return std::chrono::system_clock::now() >= exp_time;
    } catch (const std::exception& ex) {
      throw std::runtime_error("Expiration claim missing or invalid in token");
    }
  }

 private:
  std::string secret_;
  std::string issuer_;
  std::chrono::seconds default_expiration_;
};

}  // namespace vpn_manager
