export function updateControllerIp(ip) {
  return {
    type: '@user/UPDATE_CONTROLLER_IP',
    payload: {
      ip
    },
  };
}