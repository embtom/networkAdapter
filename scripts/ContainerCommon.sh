
COMMON="\
-p 80:80 \
--rm \
--mount type=bind,source=${ARTIFACTS_SHARE},target=/media/artifacts \
 network-adapter_lin_x64_builder"