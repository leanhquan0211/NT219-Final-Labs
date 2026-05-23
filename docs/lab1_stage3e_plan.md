# Stage 3E Plan — Lab 1 Closure

Stage 3E is the final audit/closure step for Lab 1.

## Why Stage 3E Exists

Stage 3D adds final verification scripts, but Lab 1 still has a few requirement gaps compared with the full assignment statement. Stage 3E is needed to decide whether to implement those remaining gaps or document them clearly as limitations.

## Stage 3E Options

### Option A — Full hardening

Implement:

1. AES-XTS
2. Dedicated no-padding KAT path for official NIST CBC/CFB/OFB/CTR vectors
3. GCM/CCM official-vector parsing with AAD/tag fields
4. Persistent nonce-reuse tracking for CTR/GCM/CCM

This is more complete, but it is riskier and takes longer.

### Option B — Close Lab 1 for now

Do not add risky core code. Instead:

1. Keep current stable implementation.
2. Keep Stage 3D final verification logs.
3. Document missing XTS/full KAT/nonce tracking as limitations.
4. Move on to Lab 2.
5. Return later if time allows.

This is faster and safer for project progress.

## Recommendation

Use Option B if the project timeline is tight, because Labs 2–6 are still large. Use Option A only if there is enough time for another full patch/test cycle.
